using Microsoft.Reporting.WinForms;
using Reporting.Shared.DataSources;
using Reporting.Shared.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace Reporting.Server
{
    public class Service : IService
    {
        public Shared.DataSources.PartnerDataSource GetPartnerDataSource()
        {
            // key element: client and server use shared assembly

            List<Shared.Models.Partner> partners = new List<Shared.Models.Partner>();
            for (int i = 0; i < 100; ++i )
            {
                partners.Add(new Partner { 
                    ID = i, 
                    Name = "Company Name " + (i.ToString("00000")),
                    Address = "455 Larkspur Dr. San Jose, CA 92926"
                });
            }

            return new Shared.DataSources.PartnerDataSource
            {
                ReportName = "PartnerListReport",
                CompanyName = "Developer @ Home Ltd.",
                Partners = partners
            };
        }

        public Shared.DataSources.BaseDataSource GetSavedReport(int reportID)
        {
            // key element: KnowType attribute of BaseDataSource

            return GetPartnerDataSource();
        }

        public byte[] GenerateSavedReportAsPDF(int reportID)
        {
            // key element: server side report generate

            using (LocalReport localReport = new LocalReport())
            {
                BaseDataSource datasource = GetSavedReport(reportID);

                if (datasource != null)
                {
                    datasource.Configure(localReport);
                    return localReport.Render("PDF");
                }
            }

            return null;
        }
    }
}
