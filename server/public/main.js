import ReconnectingWebSocket from './reconnecting-socket.js';

const socket = ReconnectingWebSocket(`ws://${window.location.host}`);
socket.onmessage = e => {
  if (display.innerText != e.data) {
    display.innerText = e.data;
  }
};

const form = document.getElementById('content');
const input = document.getElementById('input');
const display = document.getElementById('display');

form.addEventListener('submit', event => {
  event.preventDefault();
  console.log(input.value);
  const message = input.value;
  input.value = "";
  fetch('/', {
    method: 'POST',
    headers: { 
      'Content-Type': 'application/json'
    },
    body: `{ "message": "${message}" }`
  });
});