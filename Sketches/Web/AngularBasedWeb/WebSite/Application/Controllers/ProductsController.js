angular.module('app')
.controller('ProductsController', ['$scope', '$state', '$stateParams', '$http', '$modal', function ($scope, $state, $stateParams, $http, $modal) {
    console.log('home.list');
    console.log($scope);
    console.log($state);
    console.log($stateParams);

    $scope.pageSize = 10;
    $scope.pageNumber = 2;
    $scope.searchText = null;
    $scope.products = [];
    $scope.totalCount = 0;
    $scope.createdFrom = {
        value: null,
        opened: false
    };
    $scope.createdTo = {
        value: null,
        opened: false
    };

    $scope.refresh = function () {
        $http.post('DataService.ashx/Products', {
            Search: $scope.searchText,
            PageNumber: $scope.pageNumber,
            PageSize: $scope.pageSize
        }).success(function (data, status, headers, config) {
            console.log(data);

            if (data.Success) {
                $scope.products = data.Products;
                $scope.totalCount = data.TotalCount;
            }

        }).error(function (data, status, headers, config) {

        });
    };

    $scope.pageChanged = function (pn) {
        $scope.pageNumber = pn;
        $scope.refresh();
    };

    $scope.search = function () {
        $scope.refresh();
    };

    $scope.selected = function (item) {
        $modal.open({
            templateUrl: 'Application/Templates/ProductDetailPopup.html',
            controller: 'ProductDetailController',
            resolve: {
                // ProductDetailController közvetlen is használható, $stateParams-ban várja paramétereit -> emuláljuk
                $stateParams: function () {
                    return { productId: item.ID };
                }
            }
        });
    };

    $scope.open = function ($event, date) {
        $event.preventDefault();
        $event.stopPropagation();

        date.opened = true;
    }

    // első oldal betöltése
    $scope.refresh();

}]);