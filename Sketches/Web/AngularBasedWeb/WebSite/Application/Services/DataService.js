define(['Application', 'Services/ServiceMessage'], function (app) {
    app.registerProvider('dataService', function () {

        var baseAddress = 'DataService.ashx';

        this.getBaseAddress = function () {
            return baseAddress;
        };

        this.setBaseAddress = function (address) {
            baseAddress = address;
        };

        this.$get = ['$http', '$rootScope', 'messageService', function ($http, $rootScope, messageService) {
            console.log('dataService');

            return {
                changePassword: function (oldPassword, newPassword) {
                    return $http.post(baseAddress + '/ChangePassword', {
                        Username: $rootScope.User.Username,
                        OldPassword: oldPassword,
                        NewPassword: newPassword
                    }).success(function (data) {
                        console.log(data);
                        if (data && !data.Success) {
                            $rootScope.serviceErrorMessage = data.ErrorDescription;
                        }
                    });
                },
                forgottenPassword: function (email) {
                    return $http.post(baseAddress + '/ForgottenPassword', {
                        Email: email
                    }).success(function (data) {
                        console.log(data);
                    });
                },
                authentication: function (username, password) {
                    $rootScope.serviceErrorMessage = null;

                    return $http.post(baseAddress + '/Authentication', {
                        Username: username,
                        Password: password
                    }).success(function (data) {
                        console.log(data);
                        if (data && !data.Success) {
                            $rootScope.serviceErrorMessage = data.ErrorDescription;
                        }
                    });
                },
                products: function (pageNumber, pageSize, createdFrom, createdTo, searchText) {
                    return $http.post(baseAddress + '/Products', {
                        Search: searchText,
                        PageNumber: pageNumber,
                        PageSize: pageSize,
                        CreatedFrom: createdFrom,
                        CreatedTo: createdTo
                    }).success(function (data) {
                        console.log(data);
                    });
                },
                productDetail: function (productID) {
                    return $http.post(baseAddress + '/ProductDetail', {
                        ProductID: productID
                    }).success(function (data) {
                        console.log(data);
                    });
                }
            };
        }];
    });

});