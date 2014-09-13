define(['Application', 'Services/DataService'], function (app) {
    app
    .registerController('ForgottenPasswordController', ['$scope', '$state', '$rootScope', '$modal', 'dataService', function ($scope, $state, $rootScope, $modal, dataService) {

        $scope.email = null;
        $scope.message = null;

        $scope.forgottenPasswordSend = function () {
            $scope.message = null;

            dataService.forgottenPassword($scope.email)
            .success(function (data, status, headers, config) {
                if (data) {
                    if (data.Success) {
                        $scope.message = data.SuccessDescription;
                    } else {
                        $scope.message = data.ErrorDescription;
                    }
                } else {
                    $scope.message = 'Service error';
                }
            }).error(function (data, status, headers, config) {
                $scope.message = 'Service error';
            });

        }
    }]);

});