define(['Application', 'Services/DataService'], function (app) {
    app
    .registerController('AuthenticationController', ['$scope', '$state', '$rootScope', '$modal', '$stateParams', 'dataService', function ($scope, $state, $rootScope, $modal, $stateParams, dataService) {

        $scope.username = null;
        $scope.password = null;

        $scope.authentication = function () {
            console.log(dataService);

            dataService.authentication($scope.username, $scope.password)
            .success(function (data, status, headers, config) {
                if (data && data.Success) {
                    $rootScope.User = data;

                    // átirányítjuk a default vagy az authentikációt megelőző képernyőre
                    if ($stateParams && $stateParams.toName) {
                        $state.go($stateParams.toName, $stateParams.toParams);
                    } else {
                        $state.go('home.default');
                    }
                }
            }).error(function (data, status, headers, config) {

            });
        };

        $scope.forgottenPassword = function () {
            $modal.open({
                templateUrl: 'Application/Templates/ForgottenPassword.html',
                controller: 'ForgottenPasswordController'
            });
        };
    }]);

});