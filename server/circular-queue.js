const CircularQueue = size => {
  const array = [];
  let readIndex = 0;
  let writeIndex = 0;

  const nextIndex = i => {
    return ++i >= size - 1 ? 0 : i;
  }

  return {
    hasNext: () => readIndex != writeIndex,
    push: value => {
      array[writeIndex] = value;
      writeIndex = nextIndex(writeIndex);
      if (writeIndex == readIndex) {
        readIndex = nextIndex(readIndex);
      }
    },
    next: () => {
      if (readIndex == writeIndex) {
        return null;
      }
      const ret = array[readIndex];
      readIndex = nextIndex(readIndex);
      return ret;
    },
    size: () => {
      if (writeIndex > readIndex) {
        return writeIndex - readIndex;
      }
      if (readIndex > writeIndex) {
        return (writeIndex + size - readIndex);
      }
      return 0;
    },
    toString: () => `${array[readIndex]} ${array}`
  };
};

module.exports = CircularQueue;