({
    appDir: 'WebSite',
    // output directory
    dir: 'Publish',
    // clean output directory
    keepBuildDir: false,
    // optimizie all script
    skipDirOptimize: false,
    // remove original files
    removeCombined: true,

    modules: [
		{ name: 'Initializer', include: ['angular', 'Application', 'ApplicationConfig'] }
    ],

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
})