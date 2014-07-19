// http://expressjs.com/
var express = require('express');
var session = require('express-session');
var bodyParser = require('body-parser');

var app = express();

app.use(bodyParser.urlencoded());

app.use(session({
	secret: 'expressnodesessionsecret',
	resave: false,
	saveUninitialized: false,
}));

app.use('/logout', function(req, res, next) {
	req.session.destroy();
	res.redirect('/');
});

app.use('/login', function(req, res, next) {
	
	var authenticated = req.session.authenticated || false;

	if (!authenticated) {
		if (req.body.username && req.body.password) {
			if (req.body.username === 'a' && req.body.password === 'a') {
				req.session.authenticated = true;
			}
		}
	}
	
	res.redirect('/');
});

app.use(function(req, res, next) {
	var authenticated = req.session.authenticated || false;
	
	var result = null;
	if (authenticated) {
		result = '<a href="/logout">Logout</a>';
	} else {
		result = '<form action="/login" method="POST"><input name="username" /><input type="password" name="password" /><input type="submit" value="Sign-in" /></form>';
	}
	
	result = result || '';
	result += '<br/>' + JSON.stringify(req.session);
	res.send(result);
});

module.exports = app;
