angular.module('app')
.controller('AccountSettingsController', ['$scope', '$state', '$rootScope', '$modal', '$stateParams', 'dataService', function ($scope, $state, $rootScope, $modal, $stateParams, dataService) {

    $scope.currentPassword = null;
    $scope.newPassword1 = null;
    $scope.newPassword2 = null;

    $scope.changePassword = function () {

        dataService.changePassword($scope.currentPassword, $scope.newPassword1)
        .success(function (data, status, headers, config) {
            console.log(data);

            if (data && data.Success) {
                $state.reload();
            }
        }).error(function (data, status, headers, config) {

        });
    };
}]);