using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Reporting.UI
{
    public class PrinterNotFoundException : Exception
    {
        public PrinterNotFoundException(String printerName)
            :base(printerName)
        {

        }

        public PrinterNotFoundException(String printerName, Exception innerException)
            : base(printerName, innerException)
        {

        }
    }
}
