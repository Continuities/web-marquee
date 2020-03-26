// Adafruit_DotStarMatrix example for single DotStar LED matrix.
// Scrolls 'Howdy' across the matrix.

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define BUFFER_SIZE 140
#define BRIGHTNESS 32
#define DATAPIN  4
#define CLOCKPIN 5
#define SCROLL_SPEED 100

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BGR);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

char message[BUFFER_SIZE];
char next[BUFFER_SIZE];
bool hasNext = false;
int messageLength = 0;
long lastScroll = millis();

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.setTextColor(colors[0]);
  setMessage("F*ck COVID-19");
  sendState();
}

void sendState() {
  Serial.println(message);
}

void queueMessage(char*m) {
  strncpy(next, m, BUFFER_SIZE);
  hasNext = true;
}

void setMessage(char* m) {
  strncpy(message, m, BUFFER_SIZE);
  int x1, y1, w, h;
  matrix.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  messageLength = w;
}

void nextMessage() {
  if (!hasNext) {
    // Nothing queued
    return;
  }
  setMessage(next);
  strncpy(next, "", BUFFER_SIZE);
  hasNext = false;
}

char buff[BUFFER_SIZE];
int bufIndex = 0;
void readChar() {
  if(Serial.available() > 0 ){
    char c = Serial.read();
    if (c == '\n') {
      queueMessage(buff);
      bufIndex = 0;
      strncpy(buff, "", BUFFER_SIZE);
    }
    else {
      buff[bufIndex++] = c;
    }
  }
}

int x    = matrix.width();
int pass = 0;

void loop() {
  readChar();
  if (millis() - lastScroll < SCROLL_SPEED) {
    return;
  }
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(message);
  if(--x < -messageLength) {
    nextMessage();
    sendState();
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  lastScroll = millis();
}
