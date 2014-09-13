define(['angular', 'angular-couch-potato', 'angular-ui-router', 'angular-ui-boostrap'], function (angular, couchPotato) {
 
    var app = angular.module('Application', ['ui.router', 'ui.bootstrap', 'scs.couch-potato'])
    .run(['$rootScope', '$location', '$state', '$couchPotato', '$stateParams', function ($rootScope, $location, $state, $couchPotato, $stateParams) {

        app.lazy = $couchPotato;

        if (!$rootScope.User) {
            $rootScope.User = null;
        }

        $rootScope.$on('$stateChangeStart', function (event, toState, toParams, fromState, fromParams) {

            var firstState = fromState.name == '';
            var authenticationRequired = toState.authenticationRequired || false;
            var authenticated = $rootScope.User;

            if (authenticationRequired && !authenticated) {
                event.preventDefault();

                var redirectParams = { toName: toState.name, toParams: angular.copy(toParams) };
                //$state.transitionTo('authentication', redirectParams, { inherit: true, relative: $state.$current, reload: true });
                $state.go('authentication', redirectParams);
            } else if (!authenticationRequired && authenticated) {
                event.preventDefault();
            }
        });

        $rootScope.$on('$stateChangeError', function (event, toState, toParams, fromState, fromParams, error) {
            console.log(error);
        });
    }])
    .config(['$stateProvider', '$urlRouterProvider', '$couchPotatoProvider', function ($stateProvider, $urlRouterProvider, $couchPotatoProvider) {

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

    couchPotato.configureApp(app);
    return app;
});