using System;
using System.Drawing;
using System.IO.Ports;
using System.Threading;

namespace LedDisplayDeviceLibrary
{
    public class LedDisplay : IDisposable
    {
        public const int DISPLAY_WIDTH = 32;
        public const int DISPLAY_HEIGHT = 16;
        public const int DISPLAY_BUFFER_SIZE = 64;
        public const int DISPLAY_BUFFER_LINE_WIDTH = 4;
        public const int DISPLAY_UNPACKED_BUFFER_SIZE = 512;
        SerialPort serialPort;
        byte[] internalBuffer = new byte[5];
        byte[] internalReadBuffer = new byte[1];
        ManualResetEvent command_completed_event = new ManualResetEvent(false);

        public LedDisplay(String com, int baudRate)
        {
            if (String.IsNullOrWhiteSpace(com))
            {
                throw new ArgumentNullException(com);
            }

            this.serialPort = new SerialPort(com, baudRate);
            this.serialPort.DataReceived += SerialPort_DataReceived;
            this.serialPort.Open();
        }

        public void DisplayImage(byte[] buffer)
        {
            if (buffer == null)
            {
                throw new ArgumentNullException("buffer");
            }

            if (buffer.Length != DISPLAY_BUFFER_SIZE && buffer.Length != DISPLAY_UNPACKED_BUFFER_SIZE)
            {
                throw new ArgumentOutOfRangeException("buffer != DISPLAY_BUFFER_SIZE");
            }

            if (buffer.Length == DISPLAY_UNPACKED_BUFFER_SIZE)
            {
                buffer = DisplayBufferHelper.Pack(buffer);
            }

            command_completed_event.Reset();

            internalBuffer[0] = (byte)'A';

            this.serialPort.Write(internalBuffer, 0, 1);
            this.serialPort.Write(buffer, 0, DISPLAY_BUFFER_SIZE);

            command_completed_event.WaitOne(1000);
        }

        public unsafe void DisplayImage(Bitmap image, bool invert)
        {
            byte[] packedBuffer = DisplayBufferHelper.GetDisplayBufferFromImage(image, invert);

            command_completed_event.Reset();

            internalBuffer[0] = (byte)'A';

            this.serialPort.Write(internalBuffer, 0, 1);
            this.serialPort.Write(packedBuffer, 0, DISPLAY_BUFFER_SIZE);

            command_completed_event.WaitOne(1000);
        }

        public void Clear()
        {
            command_completed_event.Reset();
            this.serialPort.Write(new byte[] { (byte)'D' }, 0, 1);
            command_completed_event.WaitOne();
        }

        public void Fill()
        {
            command_completed_event.Reset();
            this.serialPort.Write(new byte[] { (byte)'E' }, 0, 1);
            command_completed_event.WaitOne();
        }

        public void ShiftRight(ushort column)
        {
            ShiftRight((byte)(column >> 8), (byte)(column));
        }

        public void ShiftRight(byte[] column)
        {
            if (column == null)
            {
                throw new ArgumentNullException("line");
            }

            if (column.Length < 2)
            {
                throw new ArgumentOutOfRangeException("line");
            }

            ShiftRight(column[1], column[0]);
        }

        public void ShiftRight(byte upper, byte lower)
        {
            this.internalBuffer[0] = (byte)'B';
            this.internalBuffer[1] = upper;
            this.internalBuffer[2] = lower;

            command_completed_event.Reset();

            this.serialPort.Write(this.internalBuffer, 0, 3);

            command_completed_event.WaitOne();
        }

        public void ShiftLeft(ushort column)
        {
            ShiftLeft((byte)(column >> 8), (byte)(column));
        }

        public void ShiftLeft(byte[] column)
        {
            if (column == null)
            {
                throw new ArgumentNullException("line");
            }

            if (column.Length < 2)
            {
                throw new ArgumentOutOfRangeException("line");
            }

            ShiftLeft(column[1], column[0]);
        }

        public void ShiftLeft(byte upper, byte lower)
        {
            this.internalBuffer[0] = (byte)'C';
            this.internalBuffer[1] = upper;
            this.internalBuffer[2] = lower;

            command_completed_event.Reset();

            this.serialPort.Write(this.internalBuffer, 0, 3);

            command_completed_event.WaitOne();
        }

        public void ShiftDown(uint line)
        {
            ShiftDown((byte)(line >> 24), (byte)(line >> 16), (byte)(line >> 8), (byte)line);
        }

        public void ShiftDown(byte[] line)
        {
            if (line == null)
            {
                throw new ArgumentNullException("line");
            }

            if (line.Length < 4)
            {
                throw new ArgumentOutOfRangeException("line");
            }

            ShiftDown(line[3], line[2], line[1], line[0]);
        }

        public void ShiftDown(byte c1, byte c2, byte c3, byte c4)
        {
            this.internalBuffer[0] = (byte)'F';
            this.internalBuffer[1] = c1;
            this.internalBuffer[2] = c2;
            this.internalBuffer[3] = c3;
            this.internalBuffer[4] = c4;

            command_completed_event.Reset();

            this.serialPort.Write(this.internalBuffer, 0, 5);

            command_completed_event.WaitOne();
        }

        public void ShiftUp(uint line)
        {
            ShiftUp((byte)(line >> 24), (byte)(line >> 16), (byte)(line >> 8), (byte)line);
        }

        public void ShiftUp(byte[] line)
        {
            if (line == null)
            {
                throw new ArgumentNullException("line");
            }

            if (line.Length < 4)
            {
                throw new ArgumentOutOfRangeException("line");
            }

            ShiftUp(line[3], line[2], line[1], line[0]);
        }

        public void ShiftUp(byte c1, byte c2, byte c3, byte c4)
        {
            this.internalBuffer[0] = (byte)'G';
            this.internalBuffer[1] = c1;
            this.internalBuffer[2] = c2;
            this.internalBuffer[3] = c3;
            this.internalBuffer[4] = c4;

            command_completed_event.Reset();

            this.serialPort.Write(this.internalBuffer, 0, 5);

            command_completed_event.WaitOne();
        }
        
        public void Test()
        {
            ShiftLeft(0xFF, 0xFF);

            for (int i = 0; i < 32; ++i)
            {
                Thread.Sleep(50);

                ShiftLeft(0x00, 0x00);
            }

            ShiftRight(0xFF, 0xFF);

            for (int i = 0; i < 32; ++i)
            {
                Thread.Sleep(50);

                ShiftRight(0x00, 0x00);
            }

            this.Fill();

            Thread.Sleep(1000);

            this.Clear();

            Thread.Sleep(1000);

            this.Fill();

            ShiftDown(0xFF, 0xFF, 0xFF, 0xFF);

            for (int i = 0; i < 32; ++i)
            {
                Thread.Sleep(50);

                ShiftDown(0x00, 0x00, 0x00, 0x00);
            }

            this.Fill();

            ShiftUp(0xFF, 0xFF, 0xFF, 0xFF);

            for (int i = 0; i < 32; ++i)
            {
                Thread.Sleep(50);

                ShiftUp(0x00, 0x00, 0x00, 0x00);
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (e.EventType == SerialData.Chars)
            {
                this.serialPort.Read(internalReadBuffer, 0, internalReadBuffer.Length);

                if (internalReadBuffer[0] != 'Z')
                {
                    command_completed_event.Set();
                }
            }
        }

        public void Dispose()
        {
            this.command_completed_event.Dispose();
            this.serialPort.Dispose();
        }
    }
}
