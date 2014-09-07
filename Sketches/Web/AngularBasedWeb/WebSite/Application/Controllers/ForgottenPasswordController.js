angular.module('app')
.controller('ForgottenPasswordController', ['$scope', '$state', '$http', '$rootScope', '$modal', function ($scope, $state, $http, $rootScope, $modal) {

    $scope.email = null;
    $scope.message = null;

    $scope.forgottenPasswordSend = function () {
        $scope.message = null;

        $http.post('DataService.ashx/ForgottenPassword', {
            Email: $scope.email
        }).success(function (data, status, headers, config) {
            console.log(data);

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