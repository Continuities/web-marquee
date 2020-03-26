const withPing = socket => {
  let pingInterval;
  let sawPong = true;

  socket.addEventListener('connection', () => {
    pingInterval = setInterval(() => {
      if (!sawPong) {
        return kill();
      }
      sawPong = false;
      socket.ping();
    }, PING_RATE);
  });

  socket.addEventListener('pong', () => {
    sawPong = true;
  });

  socket.addEventListener('close', () => clearInterval(pingInterval));

  const kill = () => {
    console.log('No pong received. Terminating socket...');
    socket.terminate();
    clearInterval(pingInterval);
  };

  return socket;
};

module.exports = withPing;