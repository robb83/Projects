angular.module('app')
.controller('ProductDetailController', ['$scope', '$stateParams', 'dataService', function ($scope, $stateParams, dataService) {
    
    console.log($stateParams);

    $scope.loading = true;
    $scope.errorMessage = null;

    $scope.product = {
        ID: $stateParams.productId
    };

    dataService.productDetail($scope.product.ID)
    .success(function (data, status, headers, config) {
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