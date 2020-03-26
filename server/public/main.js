import ReconnectingWebSocket from './reconnecting-socket.js';

const socket = ReconnectingWebSocket(`ws://${window.location.host}`);
socket.onmessage = e => {
  const [ colour, message ] = e.data.split('|');
  if (display.innerText != message) {
    display.innerText = message;
  }
  display.style.color = `#${colour}`;
};

const form = document.getElementById('content');
const input = document.getElementById('input');
const display = document.getElementById('display');
const colour = document.getElementById('colour');

function updateColour() {
  input.style.color = colour.value;
}

colour.addEventListener('change', updateColour);

form.addEventListener('submit', event => {
  event.preventDefault();
  const message = input.value;
  input.value = "";
  fetch('/', {
    method: 'POST',
    headers: { 
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      message,
      colour: colour.value.substring(1)
    })
  });
});

updateColour();