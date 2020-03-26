const SERIAL_PORT = '/dev/ttyACM0';

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const http = require('http');
const express = require('express');
const bodyParser = require('body-parser');
const WebSocket = require('ws');
const CircularQueue = require('./circular-queue.js');
const withPing = require('./with-ping.js');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
const port = 3000;
const serial = new SerialPort(SERIAL_PORT, { baudRate: 9600 });
const parser = serial.pipe(new Readline({ delimiter: '\r\n' }));
const messageQueue = CircularQueue(50);

app.use(express.static('public'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

let currentMessage = '';
parser.on('data', data => {
  currentMessage = data;
  // Send the next queued message, if there is one
  if (messageQueue.hasNext()) {
    const msg = messageQueue.next();
    serial.write(`${msg}\n`);
    console.log(`TX: [${messageQueue.size()}] "${msg}"`);
  }
  // Broadcast the current message to clients
  wss.clients.forEach(c => {
    if (c.readyState === WebSocket.OPEN) {
      c.send(currentMessage);
    }
  });
});

wss.on('connection', ws => {
  withPing(ws);
  ws.send(currentMessage);
});

app.post('/', (req, res) => {
  messageQueue.push(req.body.message);
  console.log(`RX: [${messageQueue.size()}] "${req.body.message}"`);
  res.sendStatus(200);
});

server.listen(port, () => console.log(`Server listening on port ${port}!`))