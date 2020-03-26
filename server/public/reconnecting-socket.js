import { withPingListener } from './pingpong.js';

const RECONNECT_DELAY = 100; //1000; // ms

function needsReconnect(socket) {
  return 
    !socket || 
    socket.readyState === WebSocket.CLOSED || 
    socket.readyState === WebSocket.CLOSING;
}

const ReconnectingWebSocket = url => {
  const ret = {};

  // Reconnect on focus, if necessary
  document.addEventListener('visibilitychange', () => {
    if (document.hidden || !needsReconnect(socket)) {
      return;
    }
    connect();
  });

  let socket;
  let reconnectTimeout;
  const reconnect = () => {
    console.log('Reconnecting...');
    setTimeout(connect, RECONNECT_DELAY);
  };
  const connect = () => {
    socket = withPingListener(new WebSocket(url), reconnect);
    socket.addEventListener('message', e => {
      ret.onmessage && ret.onmessage(e);
    });
    socket.addEventListener('close', reconnect);
  };

  connect();
  return ret;
};

export default ReconnectingWebSocket;