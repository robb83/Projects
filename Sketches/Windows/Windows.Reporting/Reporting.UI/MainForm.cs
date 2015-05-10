using Microsoft.Reporting.WinForms;
using Reporting.Shared;
using Reporting.Shared.DataSources;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Printing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace Reporting.UI
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.reportViewer.RefreshReport();
        }

        private void buttonPartners_Click(object sender, EventArgs e)
        {
            using (ServiceReference.ServiceClient client = new ServiceReference.ServiceClient())
            {
                PartnerDataSource datasource = client.GetPartnerDataSource();
                
                if (datasource != null)
                {
                    datasource.Configure(this.reportViewer.LocalReport);
                }
                else
                {
                    this.reportViewer.Clear();
                }

                this.reportViewer.RefreshReport();
            }
        }

        private void buttonSavedPartnerReport_Click(object sender, EventArgs e)
        {
            using (ServiceReference.ServiceClient client = new ServiceReference.ServiceClient())
            {
                BaseDataSource datasource = client.GetSavedReport(1);

                if (datasource != null)
                {
                    datasource.Configure(this.reportViewer.LocalReport);
                }
                else
                {
                    this.reportViewer.Clear();
                }

                this.reportViewer.RefreshReport();
            }
        }

        private void buttonSaveAsPDF_Click(object sender, EventArgs e)
        {
            using (ServiceReference.ServiceClient client = new ServiceReference.ServiceClient())
            {
                byte[] result = client.GenerateSavedReportAsPDF(1);

                if (result == null || result.Length == 0)
                {
                    MessageBox.Show("Something wrong!");
                }
                else
                {
                    if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                    {
                        File.WriteAllBytes(saveFileDialog.FileName, result);
                    }
                }
            }
        }

        private void buttonSavedPartnerReportSilentPrint_Click(object sender, EventArgs e)
        {
            // https://msdn.microsoft.com/en-us/library/ms252091.aspx

            using (ServiceReference.ServiceClient client = new ServiceReference.ServiceClient())
            {
                using (LocalReport localReport = new LocalReport())
                {
                    BaseDataSource datasource = client.GetSavedReport(1);

                    if (datasource != null)
                    {
                        datasource.Configure(localReport);

                        using(LocalReportPrinter localReportPrint = new LocalReportPrinter(localReport/*, "Microsoft XPS Document Writer"*/))
                        {
                            localReportPrint.Print();
                        }
                    }
                }
            }
        }
    }
}
