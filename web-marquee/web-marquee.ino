#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define BUFFER_SIZE 100
#define BRIGHTNESS 32
#define DATAPIN  4
#define CLOCKPIN 5
#define SCROLL_SPEED 100

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BGR);

typedef struct Message {
  char text[BUFFER_SIZE];
  char colour[7]; // hex colour
} Message;

Message currentMessage;
Message nextMessage;
bool hasNext = false;
int messageLength = 0;
long lastScroll = millis();

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  Message m = { "F*ck COVID-19", "ff0000" };
  setMessage(m);
  sendState();
}

void sendState() {
  Serial.print(currentMessage.colour);
  Serial.print("|");
  Serial.println(currentMessage.text);
}

void queueMessage(Message m) {
  nextMessage = m;
  hasNext = true;
}

void setMessage(Message m) {
  currentMessage = m;
  int x1, y1, w, h;
  matrix.getTextBounds(currentMessage.text, 0, 0, &x1, &y1, &w, &h);
  messageLength = w;
  matrix.setTextColor(parseHexColour(m.colour));
}

void useNextMessage() {
  if (!hasNext) {
    // Nothing queued
    return;
  }
  setMessage(nextMessage);
  hasNext = false;
}

char buff[BUFFER_SIZE + 7];
int bufIndex = 0;
void readChar() {
  if(Serial.available() > 0 ){
    char c = Serial.read();
    if (c == '\n') {
      Message toQueue;
      strncpy(toQueue.colour, buff, 6);
      toQueue.colour[6] = 0;
      strncpy(toQueue.text, buff + 7, BUFFER_SIZE);
      queueMessage(toQueue);
      bufIndex = 0;
      strncpy(buff, "", BUFFER_SIZE);
    }
    else {
      buff[bufIndex++] = c;
    }
  }
}

uint16_t parseHexColour(char *hex) {
  char rs[3], gs[3], bs[3];
  strncpy(rs, hex, 2);
  strncpy(gs, hex+2, 2);
  strncpy(bs, hex+4, 2);
  rs[2] = 0; gs[2] = 0; bs[2] = 0;
  uint8_t r = (uint8_t)strtol(rs, NULL, 16);
  uint8_t g = (uint8_t)strtol(gs, NULL, 16);
  uint8_t b = (uint8_t)strtol(bs, NULL, 16);
  return matrix.Color(r, g, b);
}

int x = matrix.width();
void loop() {
  readChar();
  if (millis() - lastScroll < SCROLL_SPEED) {
    return;
  }
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(currentMessage.text);
  if(--x < -messageLength) {
    useNextMessage();
    sendState();
    x = matrix.width();
  }
  matrix.show();
  lastScroll = millis();
}
