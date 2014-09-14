require.config({

    baseUrl: 'Application',

    paths: {
        'angular'               : '../Assets/js/angular',
        'angular-ui-router'     : '../Assets/js/angular-ui-router',
        'angular-ui-boostrap'   : '../Assets/js/angular-ui-boostrap',
        'angular-couch-potato'  : '../Assets/js/angular-couch-potato'
    },

    shim: {
        'angular'               : { exports: 'angular'},
        'angular-ui-router'     : { deps: ['angular'] },
        'angular-ui-boostrap'   : { deps: ['angular'] },
        'angular-couch-potato'  : { deps: ['angular'] }
    }
});

require(['angular', 'Application', 'ApplicationConfig'], function(angular, app) {

    angular.element(document).ready(function() {

        angular.bootstrap(document, [app['name'], function() {

            // for good measure, put ng-app on the html element
            // studiously avoiding jQuery because angularjs.org says we shouldn't
            // use it.  In real life, there are a ton of reasons to use it.
            // karma likes to have ng-app on the html element when using requirejs.
            angular.element(document).find('html').addClass('ng-app');

        }]);

    });
});