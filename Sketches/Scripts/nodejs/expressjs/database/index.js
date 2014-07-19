// http://expressjs.com/
// http://mongoosejs.com/
// http://www.sebastianseilund.com/nodejs-async-in-practice
var async = require('async');
var express = require('express');
var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/test');

var Activity = mongoose.model('Activity', { url: String, created: Number });

var app = express();

app.use(function(req, res, next) {
	
	var activity = new Activity({ url: req.originalUrl, created: new Date().getTime() });
	activity.save();
	
	var totalCount = 0,
		result = [],
		page = Math.abs(parseInt(req.query.page)) || 1,
		limit = 10,
		skip = (page - 1) * limit;
	
	async.parallel([
	
		// totalCount
		function(callback) {
			Activity.find({})
			.count(function( err, r) {
				totalCount = r;
				callback(err);
			});
		},
		
		// result
		function(callback) {
			Activity.find({})
			.sort('-created')
			.skip(skip)
			.limit(limit)
			.exec(function (err, r) {
				result = r;
				callback(err);
			});
		}
	// render
	], function(err) {
		if (err) throw err;
		
		var output = '<h1>Activities</h1>';
	
		var pages = Math.ceil(totalCount / limit);
		for ( var i = 0; i < pages; ++i) {
			output += '<a href="?page=' + (i + 1) + '">' + (i + 1) + '</a>&nbsp|&nbsp;';
		}
		
		output += '<table><tr><th>Created</th><th>URL</th></tr>';
		for( var i = 0; i < result.length; ++i) {
			output += '<tr><td>';
			output += result[i].created;
			output += '</td><td>';
			output += result[i].url;
			output += '</td></tr>';
		}
		output += '</table>';
		
		res.send(output);		
	});
});

module.exports = app;
