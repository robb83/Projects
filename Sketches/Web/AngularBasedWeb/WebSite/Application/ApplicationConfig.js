define(['Application', 'Services/MessageService', 'Services/DataService', 'Services/NavigationService', 'Directives/PasswordVerify'], function (app) {
 
    app.run(['$rootScope', '$location', '$state', '$couchPotato', '$stateParams', 'navigationService',
    function ($rootScope, $location, $state, $couchPotato, $stateParams) {

        // navigationService must inject here otherwise not initialized

        app.lazy = $couchPotato;
    }]);

    app.config(['$stateProvider', '$urlRouterProvider', '$couchPotatoProvider', 'navigationServiceProvider', function ($stateProvider, $urlRouterProvider, $couchPotatoProvider, navigationServiceProvider) {

        // state.params: 
        // Amikor navigálunk egy "állapotba", akkor csak meghatározott paraméterei lehetnek,
        // ezeket navigáció során szűri le a keretrendszer a state.params alapján
        // ezt dinamikusan az URL-ből generálja alapból, ha használunk ilyet, akkor azokat is fel kell tüntetni

        $urlRouterProvider.otherwise('/Authentication');

        $stateProvider.state('authentication', {
            url: '/Authentication',
            templateUrl: 'Application/Templates/Authentication.html',
            controller: 'AuthenticationController',
            params: { 'toName': null, 'toParams': null },
            authenticationRequired: false,
            resolve: {
                dummy1: $couchPotatoProvider.resolveDependencies(['Controllers/AuthenticationController']),
                dummy2: $couchPotatoProvider.resolveDependencies(['Controllers/ForgottenPasswordController']),
            }
        });

        $stateProvider.state('home', {
            abstract: true,
            templateUrl: 'Application/Templates/ApplicationFrame.html',
            controller: 'ApplicationFrameController',
            authenticationRequired: true,
            resolve: {
                dummy: $couchPotatoProvider.resolveDependencies(['Controllers/ApplicationFrameController']),
            }
        });

        $stateProvider.state('home.default', {
            url: '/',
            templateUrl: 'Application/Templates/Products.html',
            controller: 'ProductsController',
            authenticationRequired: true,
            resolve: {
                dummy1: $couchPotatoProvider.resolveDependencies(['Controllers/ProductsController']),
                dummy2: $couchPotatoProvider.resolveDependencies(['Controllers/ProductDetailController']),
            }
        });

        $stateProvider.state('home.product', {
            url: '/product/{productId:[0-9]+}',
            templateUrl: 'Application/Templates/ProductDetail.html',
            controller: 'ProductDetailController',
            authenticationRequired: true,
            resolve: {
                dumm1: $couchPotatoProvider.resolveDependencies(['Controllers/ProductDetailController'])
            }
        });

        $stateProvider.state('home.accountSettings', {
            url: '/AccountSettings',
            templateUrl: 'Application/Templates/AccountSettings.html',
            controller: 'AccountSettingsController',
            authenticationRequired: true,
            resolve: {
                dummy: $couchPotatoProvider.resolveDependencies(['Controllers/AccountSettingsController'])
            }
        });
    }]);

});