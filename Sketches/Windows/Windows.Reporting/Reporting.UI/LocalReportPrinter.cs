using Microsoft.Reporting.WinForms;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Printing;
using System.Globalization;
using System.IO;
using System.Text;

namespace Reporting.UI
{
    /// <summary>
    /// reference: https://msdn.microsoft.com/en-us/library/ms252091.aspx
    /// deviceinfo: https://msdn.microsoft.com/en-us/library/ms155373.aspx
    /// </summary>
    public class LocalReportPrinter : IDisposable
    {
        Warning[] warnings;
        LocalReport localReport;
        PrintDocument printDocument;
        List<MemoryStream> streams = new List<MemoryStream>();
        int currentPageIndex = 0;
        string deviceInfo;

        public LocalReportPrinter(LocalReport localReport, String printerName = null)
        {
            if (localReport == null)
            {
                throw new ArgumentNullException("localReport");
            }

            this.localReport = localReport;
            this.printDocument = new PrintDocument();
            this.printDocument.PrintPage += PrintPageEventCallback;

            // search printer by printerName
            if (!String.IsNullOrWhiteSpace(printerName))
            {
                foreach (String name in PrinterSettings.InstalledPrinters)
                {
                    if (name.Equals(printerName))
                    {
                        this.printDocument.PrinterSettings.PrinterName = printerName;

                        if (!this.printDocument.PrinterSettings.IsValid)
                        {
                            throw new InvalidPrinterException(this.printDocument.PrinterSettings);
                        }

                        this.deviceInfo = CreateDeviceInfo(this.printDocument.DefaultPageSettings);

                        return; // Success
                    }
                }

                throw new PrinterNotFoundException(printerName);
            }
            else
            {
                if (!this.printDocument.PrinterSettings.IsValid)
                {
                    throw new InvalidPrinterException(this.printDocument.PrinterSettings);
                }

                this.deviceInfo = CreateDeviceInfo(this.printDocument.DefaultPageSettings);
            }
        }

        public Warning[] Warnings
        {
            get { return this.warnings; }
        }

        private String ConvertToInchString(int inchHundreds)
        {
            return ((float)inchHundreds / 100.0f).ToString("0.00", NumberFormatInfo.InvariantInfo);
        }

        private String CreateDeviceInfo(PageSettings settings)
        {
            return
@"<DeviceInfo>
    <OutputFormat>EMF</OutputFormat>
    <PageWidth>8.5in</PageWidth>
    <PageHeight>11in</PageHeight>
    <MarginTop>0.25in</MarginTop>
    <MarginLeft>0.25in</MarginLeft>
    <MarginRight>0.25in</MarginRight>
    <MarginBottom>0.25in</MarginBottom>
</DeviceInfo>";
        }

        private Stream CreateStreamCallback(string name, string fileNameExtension, Encoding encoding, string mimeType, bool willSeek)
        {
            MemoryStream stream = new MemoryStream();
            streams.Add(stream);
            return stream;
        }

        private void PrintPageEventCallback(object s, PrintPageEventArgs ev)
        {
            Metafile pageImage = new Metafile(this.streams[this.currentPageIndex]);

            Rectangle adjustedRect = new Rectangle(
                ev.PageBounds.Left - (int)ev.PageSettings.HardMarginX,
                ev.PageBounds.Top - (int)ev.PageSettings.HardMarginY,
                ev.PageBounds.Width,
                ev.PageBounds.Height);

            ev.Graphics.FillRectangle(Brushes.White, adjustedRect);
            ev.Graphics.DrawImage(pageImage, adjustedRect);

            this.currentPageIndex++;

            ev.HasMorePages = (this.currentPageIndex < streams.Count);
        }

        public void Print()
        {
            // render report as EMF image
            this.localReport.Render("Image", deviceInfo, CreateStreamCallback, out this.warnings);

            // rewind
            foreach (Stream stream in streams)
            {
                stream.Position = 0;
            }

            // draw PrintDocument and print
            this.printDocument.Print();
        }
    
        public void Dispose()
        {
 	        if (this.printDocument != null)
            {
                this.printDocument.Dispose();

                this.printDocument = null;
            }

            if (this.streams != null)
            {
                foreach (Stream stream in streams)
                {
                    stream.Dispose();
                }

                this.streams = null;
            }
        }
    }
}
