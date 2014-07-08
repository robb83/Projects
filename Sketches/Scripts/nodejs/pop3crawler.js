var tls = require('tls');

var options = {
	'username': 'username1@gmail.com',
	'password': 'password1',
	'state': 0,
	'messages': 0,
	'buffer': null,
	'next': 1,
	'ms': false,
	'mf': false
};

var optionsForConnection = {
	'host': 'pop.gmail.com',
	'port': 995
};

var isok = function(str) {
	if (!str) return false;

	return str.indexOf('+OK') == 0;
};

var doSomething = function() {
	console.log('Do something');
};

var process = function(message) {
	// authentication
	if (options.state == 0) {
		if (!isok(message)) pop3.destroy();

		pop3.write('USER ' + options.username + '\r\n');
		options.state = 1;
	} else if (options.state == 1) {
		if (!isok(message)) pop3.destroy();

		pop3.write('PASS ' + options.password + '\r\n');
		options.state = 2;
	// stat
	} else if (options.state == 2) {
		if (!isok(message)) pop3.destroy();

		pop3.write('STAT\r\n');
		options.state = 3;
	// stat response, begin top
	} else if (options.state == 3) {
		if (!isok(message)) pop3.destroy();
		var values = message.split(' ');
		options.messages = parseInt(values[1]);
		
		if (options.messages > 0) {
			pop3.write('TOP 1 1\r\n');
			options.next = 2;
			options.state = 4;
		} else {
			pop3.destroy();
		}
	// top first line +OK...
	} else if (options.state == 4) {
		if (!isok(message)) pop3.destroy();
		options.state = 5;
	// message header + body
	} else if (options.state == 5) {
		// end of response?
		if (message.match(/^\.\s*$/)) {
			
			if (options.ms && options.mf) {
				
				pop3.write("DELE " + (options.next - 1) + '\r\n');
				options.state = 6;
				return;
			}
			
			options.ms = options.mf = false;
			
			if (options.next <= options.messages) {
				pop3.write('TOP ' + options.next + ' 1\r\n');
				options.state = 4;
				
				options.next = options.next + 1;
			} else {
				pop3.destroy();
			}
		} else {
			
			if (!options.ms) {
				if (message.match(/^Subject:.*Hello World/i)) {
					options.ms = true;
				}
			}
			
			if (!options.mf) {
				if (message.match(/^From:.*robb83@gmail\.com/i)) {
					options.mf = true;
				}
			}
		}
	} else if (options.state == 6) {
		pop3.write('QUIT\r\n');
		options.state = 7;
	} else if (options.state == 7) {
		options.state = 8;
		pop3.destroy();
		doSomething();
	} else {
		pop3.destroy();
	}
};

var pop3 = tls.connect(optionsForConnection, function() {
	// accept all kind of certification from server
});

pop3.on('data', function(message) {
	if (!message) return;
	var start = 0, end = -1;
	
	if (options.buffer) {
		message = options.buffer + message;
		options.buffer = null;
	}
	
	while ((end = message.indexOf('\n', start)) > -1) {
		process(message.substring(start, end));
		start = end + 1;
	}
	
	if (start < message.length) {
		options.buffer = message.substring(start);
	}
	
});

pop3.setEncoding('utf8');
