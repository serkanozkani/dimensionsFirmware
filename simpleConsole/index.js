
var dimensionsFirmware = require('dimensionsFirmware');

var df = dimensionsFirmware.connect(null, function (e) {

})

function callback () {

}

process.stdin.resume()
process.stdin.on('data', function (l) {
	var line = l.toString().substring(0, l.length-1);

	var args = line.split(' ');
	var command = args.shift();
	
	switch (command) {
		case "help":
			df.help(callback);
			break;
		case "reset":
			df.reset(callback);
			break;
		case "emergencyStop":
			df.emergencyStop(callback);
			break;

		case "testEndstopX":
			df.testEndstopX(callback);
			break;
		case "testMotors":
			df.testMotors(callback);
			break;

		case "calibrateX":
			df.calibrateX(callback);
			break;
		case "calibrateY":
			df.calibrateY(callback);
			break;
		case "calibrateZ":
			df.calibrateZ(callback);
			break;

		case "navigate":
			df.navigate(args[0], args[1], callback);
			break;
		case "status":
			df.status(callback);
			break;
		case "lockout":
			df.lockout(callback);
			break;
		case "pollHeatbed":
			df.pollHeatbed(callback);
			break;
		case "testHotend":
			df.testHotend(callback);
			break;
		case "testHeatbed":
			df.testHeatbed(callback);
			break;
		case "pollExtruder":
			df.pollExtruder(callback);
			break;

	}
})


df.on('line', function (l) {
	console.log ('>> '+l);
})