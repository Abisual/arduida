#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   5
#define TFT_DC   16
#define TFT_RST  17

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int maxLines = 6;
String prevLines[maxLines];

int lineHeight = 20;

// Настройки прогресс-бара
const int barWidth = 200;
const int barHeight = 8;
const int cpuBarY = 1 * lineHeight + 2;  // Под строкой 0 (CPU)
const int gpuBarY = 3 * lineHeight + 2;  // Под строкой 2 (GPU)

int lastCpuTemp = -1;
int lastGpuTemp = -1;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
}

// Вычисляет вертикальную позицию строки с учётом баров
int getLineY(int logicalIndex) {
  if (logicalIndex <= 2) return logicalIndex * lineHeight;            // CPU, пусто, GPU
  if (logicalIndex == 3) return (logicalIndex + 1) * lineHeight;      // VRAM
  if (logicalIndex >= 4) return (logicalIndex + 1) * lineHeight;      // пусто, RAM
  return logicalIndex * lineHeight;
}

// Отображение строки, если она изменилась
void updateLine(int index, const String& newLine) {
  if (index < 0 || index >= maxLines) return;

  if (prevLines[index] != newLine) {
    int y = getLineY(index);
    tft.fillRect(0, y, 320, lineHeight, ILI9341_BLACK);
    tft.setCursor(0, y);
    tft.println(newLine);
    prevLines[index] = newLine;
  }
}

// Парсинг температуры из строки (ожидается "T: XX C")
int parseTemperature(const String& line) {
  int idx = line.indexOf("T:");
  if (idx == -1) return -1;
  int end = line.indexOf("C", idx);
  if (end == -1) return -1;
  String tempStr = line.substring(idx + 2, end);
  tempStr.trim();
  return tempStr.toInt();
}

// Цвет прогресс-бара в зависимости от температуры
uint16_t getTempColor(int temp) {
  if (temp < 60) return ILI9341_GREEN;
  else if (temp < 80) return ILI9341_YELLOW;
  else return ILI9341_RED;
}

// Отрисовка прогресс-бара, если температура изменилась
void drawBar(int x, int y, int width, int height, int temp, int& lastTemp) {
  if (temp == lastTemp || temp < 0) return;  // Без изменений или ошибка
  lastTemp = temp;

  // Очистка старого бара
  tft.fillRect(x, y, width, height, ILI9341_BLACK);

  // Заполнение нового бара
  int fillW = map(constrain(temp, 0, 100), 0, 100, 0, width);
  uint16_t color = getTempColor(temp);
  tft.fillRect(x, y, fillW, height, color);
}

void loop() {
  static String buffer = "";

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    if (line == "END") {
      String newLines[maxLines];
      int count = 0;
      int last = 0;

      // Разбивка буфера по строкам
      for (int i = 0; i < buffer.length(); i++) {
        if (buffer[i] == '\n') {
          if (count >= maxLines) break;
          newLines[count++] = buffer.substring(last, i);
          last = i + 1;
        }
      }
      if (count < maxLines && last < buffer.length()) {
        newLines[count++] = buffer.substring(last);
      }

      // Обновление строк
      for (int i = 0; i < count; i++) {
        updateLine(i, newLines[i]);
      }

      // Прогресс-бары
      if (count > 0) {
        int cpuTemp = parseTemperature(newLines[0]);
        drawBar(0, cpuBarY, barWidth, barHeight, cpuTemp, lastCpuTemp);
      }

      if (count > 2) {
        int gpuTemp = parseTemperature(newLines[2]);
        drawBar(0, gpuBarY, barWidth, barHeight, gpuTemp, lastGpuTemp);
      }

      buffer = "";
      Serial.println("OK with bars");
    } else {
      buffer += line + '\n';
    }
  }
}
