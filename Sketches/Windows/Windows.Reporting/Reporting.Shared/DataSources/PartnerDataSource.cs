using Microsoft.Reporting.WinForms;
using Reporting.Shared.Models;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.Serialization;

namespace Reporting.Shared.DataSources
{
    [DataContract]
    public class PartnerDataSource : BaseDataSource
    {
        [DataMember]
        public List<Partner> Partners { get; set; }

        public override void Configure(Microsoft.Reporting.WinForms.LocalReport localReport)
        {
            Assembly currentAssembly = typeof(BaseDataSource).Assembly;

            localReport.LoadReportDefinition(currentAssembly.GetManifestResourceStream("Reporting.Shared.Reports.PartnerListReport.rdlc"));
            localReport.LoadSubreportDefinition("Footer", currentAssembly.GetManifestResourceStream("Reporting.Shared.Reports.Footer.rdlc"));
            localReport.LoadSubreportDefinition("PartnerListItemReport", currentAssembly.GetManifestResourceStream("Reporting.Shared.Reports.PartnerListItemReport.rdlc"));

            localReport.DataSources.Add(new ReportDataSource
            {
                Name = "Header",
                Value = new List<PartnerDataSource> { this }
            });

            localReport.DataSources.Add(new ReportDataSource
            {
                Name = "Partners",
                Value = this.Partners
            });
        }
    }
}
