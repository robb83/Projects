angular.module('app')
.controller('AccountSettingsController', ['$scope', '$state', '$http', '$rootScope', '$modal', '$stateParams', function ($scope, $state, $http, $rootScope, $modal, $stateParams) {

    $scope.currentPassword = null;
    $scope.newPassword1 = null;
    $scope.newPassword2 = null;

    $scope.changePassword = function () {

        $http.post('DataService.ashx/ChangePassword', {
            OldPassword: $scope.currentPassword,
            NewPassword: $scope.newPassword1
        }).success(function (data, status, headers, config) {
            console.log(data);

            $scope.currentPassword = null;
            $scope.newPassword1 = null;
            $scope.newPassword2 = null;

            if (data && data.Success) {
            }
        }).error(function (data, status, headers, config) {

        });
    };
}]);