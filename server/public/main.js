import ReconnectingWebSocket from './reconnecting-socket.js';

const SCROLL_RATE = 1 / 100; // pixels per second

const socket = ReconnectingWebSocket(`ws://${window.location.host}`);
socket.onmessage = e => {
  const [ colour, durationMillis, message ] = e.data.split('|');
  updateMessage(message, durationMillis);
  display.style.color = `#${colour}`;
};

const form = document.getElementById('content');
const input = document.getElementById('input');
const display = document.getElementById('inner-display');
const colour = document.getElementById('colour');

function updateMessage(newMessage, durationMillis) {
  display.style.animation = 'none';
  display.offsetHeight; /* trigger reflow */
  display.style.animation = null; 
  display.innerText = newMessage;
  display.style.animationDuration = `${durationMillis / 1000}s`;
}

window.update = updateMessage;

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