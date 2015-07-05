using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace LedDisplayDeviceLibrary
{
    public static class DisplayBufferHelper
    {
        public static void Clear(byte[] buffer)
        {
            for (int i = 0; i < buffer.Length; ++i)
            {
                buffer[i] = 0x00;
            }
        }

        public static byte[] Translation(byte[] buffer, int ox, int oy, byte[] temporary)
        {
            if (temporary == null)
            {
                temporary = new byte[buffer.Length];
            }

            for (int y = 0; y < LedDisplay.DISPLAY_HEIGHT; ++y)
            {
                for (int x = 0; x < LedDisplay.DISPLAY_WIDTH; ++x)
                {
                    int x2 = (x + ox);
                    int y2 = (y + oy);

                    if (x2 < 0 || x2 >= LedDisplay.DISPLAY_WIDTH || y2 < 0 || y2 >= LedDisplay.DISPLAY_HEIGHT)
                    {
                        continue;
                    }

                    temporary[y2 * LedDisplay.DISPLAY_WIDTH + x2] = buffer[y * LedDisplay.DISPLAY_WIDTH + x];
                }
            }

            return temporary;
        }

        public static byte[] Scale(byte[] buffer, double scale, int ox, int oy, byte[] temporary)
        {
            if (temporary == null)
            {
                temporary = new byte[buffer.Length];
            }

            for (int y = 0; y < LedDisplay.DISPLAY_HEIGHT; ++y)
            {
                for (int x = 0; x < LedDisplay.DISPLAY_WIDTH; ++x)
                {
                    int x2 = (int)((x - ox) * scale) + ox;
                    int y2 = (int)((y - oy) * scale) + oy;

                    if (x2 < 0 || x2 >= LedDisplay.DISPLAY_WIDTH || y2 < 0 || y2 >= LedDisplay.DISPLAY_HEIGHT)
                    {
                        continue;
                    }

                    temporary[y2 * LedDisplay.DISPLAY_WIDTH + x2] = buffer[y * LedDisplay.DISPLAY_WIDTH + x];
                }
            }

            return temporary;
        }

        public static byte[] Rotate(byte[] buffer, double angleInRadian, int ox, int oy, byte[] temporary)
        {
            if (temporary == null)
            {
                temporary = new byte[buffer.Length];
            }
            
            for (int y = 0; y < LedDisplay.DISPLAY_HEIGHT; ++y)
            {
                for (int x = 0; x < LedDisplay.DISPLAY_WIDTH; ++x)
                {
                    var cosAngle = Math.Cos(angleInRadian);
                    var sinAngle = Math.Sin(angleInRadian);

                    int x1 = x - ox;
                    int y1 = y - oy;

                    int x2 = (int)(cosAngle * x1 - sinAngle * y1) + 16;
                    int y2 = (int)(sinAngle * x1 + cosAngle * y1) + 7;
                    
                    if (x2 < 0 || x2 >= LedDisplay.DISPLAY_WIDTH || y2 < 0 || y2 >= LedDisplay.DISPLAY_HEIGHT)
                    {
                        continue;
                    }

                    temporary[y2 * LedDisplay.DISPLAY_WIDTH + x2] = buffer[y * LedDisplay.DISPLAY_WIDTH + x];
                }
            }

            return temporary;
        }

        public static byte[] Invert(byte[] buffer)
        {
            byte[] result = new byte[buffer.Length];
            for(int i = 0; i < result.Length; ++i)
            {
                result[i] = (byte)(~buffer[i]);
            }
            return result;
        }

        public static byte[] Pack(byte[] unpacked)
        {
            byte[] packed = new byte[64];

            for (int i = 0, b = 0; i < unpacked.Length; i += 8, ++b)
            {
                byte result = 0;

                for (int c = 0; c < 8; ++c)
                {
                    if (unpacked[i + c] > 0)
                    {
                        result |= (byte)(1 << (7 - c));
                    }
                }

                packed[b] = result;
            }

            return packed;
        }

        public unsafe static byte[] GetBufferFromImage(Bitmap image, bool invert)
        {
            if (image == null)
            {
                throw new ArgumentNullException("image");
            }

            if (image.PixelFormat != PixelFormat.Format1bppIndexed)
            {
                throw new ArgumentOutOfRangeException("image");
            }

            int w = Math.Min(LedDisplay.DISPLAY_WIDTH, image.Width);
            int h = Math.Min(LedDisplay.DISPLAY_HEIGHT, image.Height);

            byte[] buffer = new byte[LedDisplay.DISPLAY_UNPACKED_BUFFER_SIZE];
            BitmapData bitmapData = null;
            try
            {
                bitmapData = image.LockBits(
                    new Rectangle(0, 0, image.Width, image.Height),
                    ImageLockMode.ReadOnly,
                    PixelFormat.Format1bppIndexed);

                for (int y = 0; y < h; ++y)
                {
                    int p = y * LedDisplay.DISPLAY_WIDTH;
                    for (int c = 0; c < ((w + 7) / 8); ++c)
                    {
                        byte data = *(byte*)(bitmapData.Scan0 + (y * bitmapData.Stride) + c);

                        if (invert)
                        {
                            data = (byte)(~data);
                        }

                        if (c * 8 + 8 > w)
                        {
                            data = (byte)(data & (~((byte)Math.Pow(2, (c * 8 + 8) - w) - 1)));
                        }

                        for(int i = 7; i >= 0; --i)
                        {
                            buffer[p++] = (byte)(((data >> i) & 0x01) > 0 ? 0x01 : 0x00);
                        }
                    }
                }
            }
            finally
            {
                if (bitmapData != null)
                {
                    image.UnlockBits(bitmapData);
                }
            }

            return buffer;
        }

        public unsafe static byte[] GetDisplayBufferFromImage(Bitmap image, bool invert)
        {
            if (image == null)
            {
                throw new ArgumentNullException("image");
            }

            if (image.PixelFormat != PixelFormat.Format1bppIndexed)
            {
                throw new ArgumentOutOfRangeException("image");
            }

            int w = Math.Min(LedDisplay.DISPLAY_WIDTH, image.Width);
            int h = Math.Min(LedDisplay.DISPLAY_HEIGHT, image.Height);
            
            byte[] packedBuffer = new byte[LedDisplay.DISPLAY_BUFFER_SIZE];
            BitmapData bitmapData = null;
            try
            {
                bitmapData = image.LockBits(
                    new Rectangle(0, 0, image.Width, image.Height), 
                    ImageLockMode.ReadOnly, 
                    PixelFormat.Format1bppIndexed);

                for (int y = 0; y < h; ++y)
                {
                    int p = y * LedDisplay.DISPLAY_BUFFER_LINE_WIDTH;
                    for (int c = 0; c < ((w + 7) / 8); ++c)
                    {
                        byte data = *(byte*)(bitmapData.Scan0 + (y * bitmapData.Stride) + c);

                        if (invert)
                        {
                            data = (byte)(~data);
                        }
                        
                        if (c * 8 + 8 > w)
                        {
                            data = (byte)(data & (~((byte)Math.Pow(2, (c * 8 + 8) - w) - 1)));
                        }

                        packedBuffer[p++] = data;
                    }
                }
            }
            finally
            {
                if (bitmapData != null)
                {
                    image.UnlockBits(bitmapData);
                }
            }

            return packedBuffer;
        }
    }
}
