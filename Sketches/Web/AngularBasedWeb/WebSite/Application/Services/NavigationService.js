define(['Application'], function (app) {
    app.registerProvider('navigationService', function () {
        this.$get = ['$http', '$rootScope', '$state', function ($http, $rootScope, $state) {
            console.log('navigationService');

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
        }];
    });
});