const SerialPort = require('serialport');
const express = require('express');
const bodyParser = require('body-parser')

const app = express();
const port = 3000;
const serial = new SerialPort('/dev/ttyACM0', { baudRate: 9600 });

app.use(express.static('public'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.post('/', (req, res) => {
  console.log(`RX: ${req.body.message}`);
  serial.write(`${req.body.message}\n`);
  res.sendStatus(200);
});

app.listen(port, () => console.log(`Server listening on port ${port}!`))