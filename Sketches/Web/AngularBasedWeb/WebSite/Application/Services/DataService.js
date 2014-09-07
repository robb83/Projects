angular.module('app')
.provider('dataService', function () {

    var baseAddress = 'DataService.ashx';

    this.getBaseAddress = function () {
        return baseAddress;
    };

    this.setBaseAddress = function (address) {
        baseAddress = address;
    };

    this.$get = ['$http', '$rootScope', function ($http, $rootScope) {
        console.log('dataService');

        return {
            changePassword: function (oldPassword, newPassword) {
                return $http.post(baseAddress + '/ChangePassword', {
                    Username: $rootScope.User.Username,
                    OldPassword: oldPassword,
                    NewPassword: newPassword
                });
            },
            forgottenPassword: function (email) {
                return $http.post(baseAddress + '/ForgottenPassword', {
                    Email: email
                });
            },
            authentication: function (username, password) {
                return $http.post(baseAddress + '/Authentication', {
                    Username: username,
                    Password: password
                });
            },
            products: function (pageNumber, pageSize, createdFrom, createdTo, searchText) {
                return $http.post(baseAddress + '/Products', {
                    Search: searchText,
                    PageNumber: pageNumber,
                    PageSize: pageSize,
                    CreatedFrom: createdFrom,
                    CreatedTo: createdTo
                });
            },
            productDetail: function (productID) {
                return $http.post(baseAddress + '/ProductDetail', {
                    ProductID: productID
                });
            }
        };
    }];
});