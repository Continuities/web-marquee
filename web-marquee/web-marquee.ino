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
#define MESSAGE_QUEUE 3

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BGR);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

char message[BUFFER_SIZE];
char messageQueue[MESSAGE_QUEUE][BUFFER_SIZE];
int queueReadIndex = 0;
int queueWriteIndex = 0;
int messageLength = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.setTextColor(colors[0]);
  setMessage("F*ck COVID-19");
  Serial.println(message);
}

void queueMessage(char*m) {
  strncpy(messageQueue[queueWriteIndex++], m, BUFFER_SIZE);
  if (queueWriteIndex >= MESSAGE_QUEUE - 1) {
    queueWriteIndex = 0;
  }
}

void setMessage(char* m) {
  strncpy(message, m, BUFFER_SIZE);
  int x1, y1, w, h;
  matrix.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  messageLength = w;
}

void nextMessage() {
  if (queueWriteIndex == queueReadIndex) {
    // Nothing queued
    return;
  }
  setMessage(messageQueue[queueReadIndex++]);
  if (queueReadIndex >= MESSAGE_QUEUE - 1) {
    queueReadIndex = 0;
  }
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
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(message);
  if(--x < -messageLength) {
    nextMessage();
    Serial.println(message);
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
}
