define(['Application'], function (app) {
    app.registerProvider('messageService', function () {
        this.$get = ['$http', '$rootScope', function ($http, $rootScope) {
            console.log('messageService');
        }];
    });
});