angular.module('app')
.controller('ProductDetailController', ['$scope', '$http', '$stateParams', function ($scope, $http, $stateParams) {
    
    console.log($stateParams);

    $scope.loading = true;
    $scope.errorMessage = null;

    $scope.product = {
        ID: $stateParams.productId
    };

    $http.post('DataService.ashx/ProductDetail', {
        ProductID: $scope.product.ID
    }).success(function (data, status, headers, config) {
        console.log(data);

        $scope.loading = false;
        if (data.Success) {
            $scope.product = data.Product;
        } else {
            $scope.errorMessage = data.errorDescription;
        }

    }).error(function (data, status, headers, config) {
        $scope.loading = false;
        $scope.errorMessage = 'Service error';
    });

}]);