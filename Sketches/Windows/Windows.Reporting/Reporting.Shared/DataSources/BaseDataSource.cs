using Microsoft.Reporting.WinForms;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Reporting.Shared.DataSources
{
    [DataContract]
    [KnownType(typeof(PartnerDataSource))]
    public class BaseDataSource
    {
        [DataMember]
        public String ReportName { get; set; }

        [DataMember]
        public String CompanyName { get; set; }

        public virtual void Configure(LocalReport reportViewer)
        {
            throw new NotImplementedException();
        }
    }
}
