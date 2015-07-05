using LedDisplayDeviceLibrary;
using System;
using System.Collections.Generic;
using System.Drawing;
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
                display.DisplayImage((Bitmap)Image.FromFile(@"letter2.bmp"), true);

                Thread.Sleep(3000);

                display.Test();

                Thread.Sleep(1000);

                Translation(display, (Bitmap)Image.FromFile(@"letter2.bmp"));

                Rotate(display, (Bitmap)Image.FromFile(@"letter.bmp"));

                Scale(display, (Bitmap)Image.FromFile(@"letter.bmp"));
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
