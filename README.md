# arduida

It is a mini-project that displays key computer parameters (temperatures, CPU and GPU utilization, RAM and VRAM) on a display connected to a microcontroller. The most important thing is that it works with Ryzen processors.

Monitoring is carried out using [AIDA64](https://www.aida64.com/). A Python script reads data from shared memory and sends it over the COM port to an Arduino (or ESP32), where the data is visualized.

---

## ⚙ Technology Stack

- **Python 3.10+**
- **AIDA64**
- **Arduino (Nano / Uno / Mega / etc.) or ESP32**
- **OLED display** (0.96", I2C, SSD1306) — any model will work or ILI9341 for esp32
- **Arduino Libraries**: `Adafruit_GFX`, `Adafruit_SSD1306` or `Adafruit_ILI9341`
- **Python Libraries**: `pyserial`, `python_aida64`

---

## 📦 Installation & Setup

### 1. Install Python libraries

```bash
pip install pyserial
pip install git+https://github.com/gwy15/python_aida64
```

The memory reading logic is sourced from: https://github.com/gwy15/python_aida64

### Step 2: Enable AIDA64 Shared Memory Export

You can configure AIDA64 to export any sensor values to shared memory.  
Details:
- [AIDA64 External Applications](https://www.aida64.com/user-manual/hardware-monitoring/external-applications)
- [Complete Sensor List](https://www.aida64.com/user-manual/hardware-monitoring/external-applications/complete-sensor-value-list)

---

## 📜 `main.py` (on PC)

The script:

- Connects to AIDA64 shared memory
- Extracts CPU/GPU temperature, load, VRAM, and RAM data
- Sends the data to Arduino or ESP32 via COM port every second

Make sure the correct **COM port** is set in `main.py`:

```python
serial.Serial("COM3", 9600)  # ← replace COM3 with your port!
```

---

## 🔌 Uploading Sketch to Arduino

### Step 1: Install libraries via Arduino IDE

- **Adafruit SSD1306**
- **Adafruit GFX**

---

## 🔌 Using ESP32 + ILI9341 (TFT)

For larger and colorful display output, you can use an ESP32 CP2102 38-pin board with a 2.8" 320x240 TFT LCD (ILI9341).

### ESP32 setup:

- **Board**: ESP32 Dev Module (CP2102, 38-pin)
- **Display**: ILI9341 320x240 (SPI)
- **Libraries**:
  - `Adafruit_GFX`
  - `Adafruit_ILI9341`

### Features:

- Full-screen layout
- CPU and GPU progress bars with color indicators:
  - Green: <60°C
  - Yellow: 60–80°C
  - Red: >80°C
- Clean, non-flickering updates
- Dynamic layout adjustments for temperature bars

The sketch reads multi-line data blocks from the PC, parses and displays them in smooth updates.

---

## ⚙️ Data format between PC and microcontroller

The PC sends the following format once per second:

```
CPU T: 52 C L: 15%
(blank line)
GPU T: 43 C L: 25%
VRAM 8142/12128
(blank line)
RAM: 16328/48392
```

---

## ✅ Result

On the display:

```
CPU T: 52 C L: 15%
[temperature bar]

GPU T: 43 C L: 25%
[temperature bar]
VRAM 8142/12128

RAM: 16328/48392
```

---

## Notes:
- If AIDA64 is not running, the Python script cannot access shared memory.
- Recommended: upload the Arduino/ESP32 sketch first, then launch the Python script.
- While debugging: always restart Arduino/ESP32 before Python script to ensure sync.

---

## 🧩 ESP32 + ILI9341 Wiring

Standard SPI wiring scheme for connecting the ILI9341 TFT display to ESP32 (38-pin):

| ILI9341 Pin   | ESP32 Signal     | ESP32 GPIO     |
|---------------|------------------|----------------|
| VCC           | 3.3V             | 3.3V           |
| GND           | GND              | GND            |
| CS            | Chip Select      | GPIO5          |
| RESET         | Reset            | GPIO17         |
| DC (A0)       | Data/Command     | GPIO16         |
| MOSI (SDI)    | SPI Data         | GPIO23         |
| SCK           | SPI Clock        | GPIO18         |
| LED           | Backlight        | 3.3V (via 220Ω resistor) |

📌 **Note:** Use 3.3V logic level for all connections. No level shifter needed.
