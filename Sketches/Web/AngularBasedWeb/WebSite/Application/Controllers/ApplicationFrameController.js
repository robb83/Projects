angular.module('app')
.controller('FrameController', ['$scope', '$state', '$stateParams', '$rootScope', function ($scope, $state, $stateParams, $rootScope) {
    console.log('home');
    console.log($scope);
    console.log($state);
    console.log($stateParams);

    $scope.logout = function () {
        $rootScope.User = null;
        $state.go('authentication');
    }
}]);