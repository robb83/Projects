angular.module('app')
.controller('AuthenticationController', ['$scope', '$state', '$http', '$rootScope', '$modal', '$stateParams', function ($scope, $state, $http, $rootScope, $modal, $stateParams) {

    $scope.username = null;
    $scope.password = null;

    $scope.authentication = function () {

        $http.post('DataService.ashx/Authentication', {
            Username: $scope.username,
            Password: $scope.password
        }).success(function (data, status, headers, config) {
            console.log(data);

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