// Adafruit_DotStarMatrix example for single DotStar LED matrix.
// Scrolls 'Howdy' across the matrix.

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define BUFFER_SIZE 32
#define BRIGHTNESS 32
#define DATAPIN  4
#define CLOCKPIN 5

// MATRIX DECLARATION:
// Parameter 1 = width of DotStar matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   DS_MATRIX_TOP, DS_MATRIX_BOTTOM, DS_MATRIX_LEFT, DS_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     DS_MATRIX_TOP + DS_MATRIX_LEFT for the top-left corner.
//   DS_MATRIX_ROWS, DS_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   DS_MATRIX_PROGRESSIVE, DS_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type:
//   DOTSTAR_BRG  Pixels are wired for BRG bitstream (most DotStar items)
//   DOTSTAR_GBR  Pixels are wired for GBR bitstream (some older DotStars)

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_BGR);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

char message[BUFFER_SIZE];
char nextMessage[BUFFER_SIZE];
bool newMessage = false;
int messageLength = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.setTextColor(colors[0]);
  setMessage("FUCK COVID");
}

void setNextMessage(char* m) {\
  strncpy(nextMessage, m, BUFFER_SIZE);
  newMessage = true;
}

void setMessage(char* m) {
  strncpy(message, m, BUFFER_SIZE);
  int x1, y1, w, h;
  matrix.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  messageLength = w;
}

char buff[BUFFER_SIZE];
int bufIndex = 0;
void readChar() {
  if(Serial.available() > 0 ){
    char c = Serial.read();
    if (c == '\n') {
      setNextMessage(buff);
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
    if (newMessage) {
      setMessage(nextMessage);
      newMessage = false;
    }
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
}
