// https://github.com/expressjs/session
var express = require('express');
var session = require('express-session');

var app = express();

app.use(session({ 
	secret: 'expressnodesessionsecret',
	resave: false,
	saveUninitialized: false,
}));

app.use(function(req, res, next) {
	var views = req.session.views || 0;
	req.session.views = ++views;
    res.send('Hello World! <br/> Page views: <br/>' + JSON.stringify(req.session));
});

module.exports = app;
