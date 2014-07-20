var child = require('child_process');
var os = require('os');
var fs = require('fs');

var numberOfChild = os.cpus.length || 2;
var workers = [];
var numbers = 0, bucket = [];

console.log('Number of cpus: ' + numberOfChild);

for( var i = 0; i < numberOfChild; ++i) {
	var worker = child.fork('./worker.js');
	workers.push(worker);

	worker.on('message', function(msg) {
		if (msg && msg.p === true) {
			bucket.push(msg.n);
		}
		
		this.send(++numbers);
	});
}

setInterval(function() {
	console.log(bucket.length);
	if (bucket.length > 0) {
		
		fs.appendFile('primes.txt', JSON.stringify(bucket));
		bucket = [];
	}
}, 5000);
