const PING_RATE = 1000; // ms

export const withPing = socket => {
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

export const withPingListener = (socket, onTerminate) => {
  let pingInterval;
  let sawPing = true;

  socket.addEventListener('connection', () => {
    pingInterval = setInterval(() => {
      if (!sawPing) {
        kill();
      }
      sawPing = false;
    }, PING_RATE * 2);

    socket.addEventListener('ping', () => {
      sawPing = true;
    });

    socket.addEventListener('close', () => clearInterval(pingInterval));

    const kill = () => {
      console.log('No ping received. Terminating socket...');
      socket.terminate();
      onTerminate && onTerminate();
      clearInterval(pingInterval);
    };
  });

  return socket;
};