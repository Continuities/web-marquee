import ReconnectingWebSocket from './reconnecting-socket.js';

const SCROLL_RATE = 1 / 100; // pixels per second

const socket = ReconnectingWebSocket(`ws://${window.location.host}`);
socket.onmessage = e => {
  const [ colour, message ] = e.data.split('|');
  updateMessage(message);
  display.style.color = `#${colour}`;
};

const form = document.getElementById('content');
const input = document.getElementById('input');
const display = document.getElementById('inner-display');
const colour = document.getElementById('colour');

function updateMessage(newMessage) {
  if (display.innerText === newMessage) {
    return;
  }
  display.style.animation = 'none';
  display.offsetHeight; /* trigger reflow */
  display.style.animation = null; 
  display.innerText = newMessage;
  const width = display.clientWidth;
  const scrollDuration = SCROLL_RATE * width;
  display.style.animationDuration = `${scrollDuration}s`;
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