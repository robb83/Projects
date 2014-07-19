#!/usr/bin/env nodejs
var fs = require('fs');

if (process.argv.length < 3 || !fs.existsSync('./' + process.argv[2])) {
	
	console.log('Usage: nodejs index.js module_directory_name');
	
	process.exit();
	return;
}

var app = require('./' + process.argv[2]);

app.set('port', process.env.PORT || 3000);
var server = app.listen(app.get('port'), function() {
  console.log('Express server listening on port ' + server.address().port);
});

