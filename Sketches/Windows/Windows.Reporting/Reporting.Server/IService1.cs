using Reporting.Shared.DataSources;
using System.ServiceModel;

namespace Reporting.Server
{
    [ServiceContract]
    public interface IService
    {
        [OperationContract]
        PartnerDataSource GetPartnerDataSource();

        [OperationContract]
        BaseDataSource GetSavedReport(int reportID);

        [OperationContract]
        byte[] GenerateSavedReportAsPDF(int reportID);
    }
}
