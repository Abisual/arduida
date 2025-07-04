#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String lines[6];
int currentLine = 0;
bool dataReady = false;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void drawLines() {
  display.clearDisplay();
  display.setCursor(0, 0);
  for (int i = 0; i < 6; i++) {
    display.println(lines[i]);
  }
  display.display();
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      currentLine++;
      if (currentLine == 6) {
        currentLine = 0;
        dataReady = true;
        drawLines();  // только когда пришли все 6 строк
        for (int i = 0; i < 6; i++) lines[i] = ""; // очистка буфера строк
      }
    } else {
      if (currentLine < 6) {
        lines[currentLine] += c;
      }
    }
  }

  // Ничего не делаем в loop, пока не получим новых 6 строк
}
