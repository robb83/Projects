using LedDisplayDeviceLibrary;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace DeviceTestClient
{
    public class Program
    {
        public void Main(string[] args)
        {
            using(LedDisplay display = new LedDisplay("COM3", 250000))
            {
                display.Clear();
                HappyHoliday(display);
            }
        }

        public void HappyHoliday(LedDisplay display)
        {
            while (true)
            {
                for (int a = 0; a < 2; ++a)
                {
                    PrintText(display, "Kellemes ünnepeket!", 50, 4, false);
                    PrintText(display, "    ", 50, 4, false);
                }

                Snow(display, 100000000 /* 10 sec */);
            }
        }

        public void Test(LedDisplay display)
        {
            display.DisplayImage((Bitmap)Image.FromFile(@"letter2.bmp"), true);

            Thread.Sleep(3000);

            display.Test();

            Thread.Sleep(1000);

            Translation(display, (Bitmap)Image.FromFile(@"letter2.bmp"));

            Rotate(display, (Bitmap)Image.FromFile(@"letter.bmp"));

            Scale(display, (Bitmap)Image.FromFile(@"letter.bmp"));
        }

        public void Snow(LedDisplay display, long timeout)
        {
            Random r = new Random();
            long start = DateTime.Now.Ticks;

            while (true)
            {
                uint line = 0x00;

                for(int a = 0; a < 32; ++a)
                {
                    line = (uint)(line << 1) | (uint)(r.Next(100) < 5 ? (uint)1 : (uint)0);
                }

                display.ShiftDown(line);

                if (DateTime.Now.Ticks - start < timeout)
                {
                    Thread.Sleep(100);
                }
                else
                {
                    break;
                }
            }

            for (int a = 0; a < 16; ++a)
            {
                display.ShiftDown(0x00);
                Thread.Sleep(100);
            }
        }

        public void PrintText(LedDisplay display, String text, int speed, int offset, bool underline)
        {
            ushort lastColumnPattern = 0x00;
            ushort underlinePattern = 0x00;

            if (offset > 0)
            {
                underlinePattern = (ushort)(0x01 << (offset - 1));
            }

            for (int i = 0; i < text.Length; ++i)
            {
                char ch = text[i];
                Boolean whitespace = Char.IsWhiteSpace(ch);

                byte[] pattern = DefaultDisplayFont.GetCharacterBitmap(ch);

                for (int b = 0; b < 8; ++b)
                {
                    ushort c = (ushort)(pattern[b] << offset);

                    // Kerning?
                    if (!whitespace)
                    {
                        if (c == 0x00 && lastColumnPattern == 0x00) continue;
                        lastColumnPattern = c;
                    }
                    
                    if (underline)
                    {
                        c |= underlinePattern;
                    }

                    display.ShiftLeft(c);

                    Thread.Sleep(speed);
                }

                // space between characters
                if (lastColumnPattern != 0x00)
                {
                    if (underline)
                    {
                        display.ShiftLeft(underlinePattern);
                    }
                    else
                    {
                        display.ShiftLeft(0x0000);
                    }

                    Thread.Sleep(speed);
                }
            }
        }

        public void Translation(LedDisplay display, Bitmap image)
        {
            int originalImageWidth = image.Width;
            byte[] originalImage = DisplayBufferHelper.GetBufferFromImage(image, true);
            byte[] temporaryImage = new byte[LedDisplay.DISPLAY_UNPACKED_BUFFER_SIZE];

            int ox = 0;
            int direction = 1;

            long start = DateTime.Now.Ticks;
            while (DateTime.Now.Ticks - start < 10 * 1000 * 10000)
            {

                DisplayBufferHelper.Clear(temporaryImage);
                DisplayBufferHelper.Translation(originalImage, ox, 0, temporaryImage);

                display.DisplayImage(temporaryImage);

                Thread.Sleep(50);

                ox += direction;

                if (ox + originalImageWidth > LedDisplay.DISPLAY_WIDTH || ox < 0)
                {
                    direction = direction * -1;
                    ox += direction;
                }
            }
        }

        public void Scale(LedDisplay display, Bitmap image)
        {
            byte[] originalImage = DisplayBufferHelper.GetBufferFromImage(image, true);
            byte[] temporaryImage = new byte[LedDisplay.DISPLAY_UNPACKED_BUFFER_SIZE];

            double angle = Math.PI / 2;
            long start = DateTime.Now.Ticks;
            while (DateTime.Now.Ticks - start < 10 * 1000 * 10000)
            {

                DisplayBufferHelper.Clear(temporaryImage);
                DisplayBufferHelper.Scale(originalImage, Math.Sin(angle), 16, 7, temporaryImage);

                display.DisplayImage(temporaryImage);

                Thread.Sleep(50);

                angle += 0.1;

                if (angle >= Math.PI)
                {
                    angle = 0.0d;
                }
            }
        }

        public void Rotate(LedDisplay display, Bitmap image)
        {
            byte[] originalImage = DisplayBufferHelper.GetBufferFromImage(image, true);
            byte[] temporaryImage = new byte[LedDisplay.DISPLAY_UNPACKED_BUFFER_SIZE];
            
            double angle = 0.0d;
            long start = DateTime.Now.Ticks;
            while (DateTime.Now.Ticks - start < 10 * 1000 * 10000)
            {
                if (angle >= Math.PI)
                {
                    angle = 0.0d;
                }

                DisplayBufferHelper.Clear(temporaryImage);
                DisplayBufferHelper.Rotate(originalImage, angle, 16, 7, temporaryImage);

                display.DisplayImage(temporaryImage);                

                if (angle == 0.0d)
                {
                    Thread.Sleep(2000);
                }
                else
                {
                    Thread.Sleep(30);
                }

                angle += 0.1;
            }
        }
    }
}
