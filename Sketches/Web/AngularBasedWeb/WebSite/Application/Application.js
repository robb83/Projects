define(['angular', 'angular-couch-potato', 'angular-ui-router', 'angular-ui-boostrap'],
function (angular, couchPotato) {
    // create angular application
    var app = angular.module('Application', ['scs.couch-potato', 'ui.router', 'ui.bootstrap']);
    // extend app with couch potato
    couchPotato.configureApp(app);
    // return for requirejs
    return app;
});