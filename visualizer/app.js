let file = './data/TRACK.TXT';

const GPS = require('gps');
const fs = require('fs');
/*const SerialPort = require('serialport'); //TODO: Remove?
const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
    delimiter: '\r\n'
});

const port = new SerialPort(file, {
    baudRate: 9600
});*/

//port.pipe(parser);

let gps = new GPS;

gps.on('data', function(data) {
    console.log(data);
});

fs.readFile(file, "utf-8", (err, data) => {
    //console.log(data);
    for(let line in data.split('\n')) {
        gps.update(line.trim());
    }
});

/*port.on('data', function(data) {
    gps.update(data);
});*/