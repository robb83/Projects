function isPrime(number) {
	if (number < 10) {
		
		if (number === 0 || number === 1 || number === 9) 
			return false;
		if (number === 2 || number === 3 || number === 5 || number === 7)
			return true;
	}
	
	if (number % 2 === 0) return false;
	if (number % 5 === 0) return false;
	
	var limit = Math.sqrt(number);
	for( var i = 3; i <= limit; i += 2) {
		if ((number % i) === 0) {
			return false;
		}
	}
	
	return true;
}

var timeout = setTimeout(process.exit, 1000);

process.on('message', function(number) {
	clearTimeout(timeout);
	
	process.send({ n: number, p: isPrime(number)});	
	timeout = setTimeout(process.exit, 1000);
});

// start cycle
process.send(null);
