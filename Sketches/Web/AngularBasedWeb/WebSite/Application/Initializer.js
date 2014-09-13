require.config({

    baseUrl: 'Application',

    paths: {
        'angular'               : '../Assets/js/angular',
        'angular-ui-router'     : '../Assets/js/angular-ui-router',
        'angular-couch-potato'  : '../Assets/js/angular-couch-potato',
        'angular-ui-boostrap'   : '../Assets/js/angular-ui-boostrap'
    },

    shim: {
        'angular': { exports: 'angular' },
        'angular-ui-router': { deps: ['angular'] },
        'angular-ui-boostrap': { deps: ['angular'] }
    }
});

require(['Application', 'angular'], function(app, angular) {

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
//<script src="Assets/js/angular.js"></script>
//<script src="Assets/js/angular-ui-router.js"></script>
//<script src="Assets/js/ui-bootstrap-tpls-0.11.0.min.js"></script>
//<script src="Application/Services/ControllerResolver.js"></script>
//<script src="Application/Application.js"></script>
//<script src="Application/Services/ServiceMessage.js"></script>
//<script src="Application/Services/DataService.js"></script>
//<script src="Application/Directives/PasswordVerify.js"></script>
//<script src="Application/Controllers/ApplicationFrameController.js"></script>
//<script src="Application/Controllers/AuthenticationController.js"></script>
//<script src="Application/Controllers/ProductDetailController.js"></script>
//<script src="Application/Controllers/ProductsController.js"></script>
//<script src="Application/Controllers/ForgottenPasswordController.js"></script>
//<script src="Application/Controllers/AccountSettingsController.js"></script>