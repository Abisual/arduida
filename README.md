# arduida
It is a mini-project that displays key computer parameters (temperatures, CPU and GPU utilization, RAM and VRAM) on an OLED display connected to an Arduino. The most important thing is that it works with Ryzen processors.

Мониторинг осуществляется через [AIDA64](https://www.aida64.com/), Python-скрипт считывает данные из общей памяти и передаёт их через COM-порт на Arduino, где данные визуализируются.

---

## ⚙ Стек технологий

- **Python 3.10+**
- **AIDA64** (Extreme или Engineer)
- **Arduino (Nano / Uno / Mega / др.)**
- **OLED-дисплей** (0.96", I2C, SSD1306)
- **Библиотеки Arduino**: `Adafruit_GFX`, `Adafruit_SSD1306`
- **Библиотеки Python**: `pyserial`, `python_aida64`

---

## 📦 Установка и настройка

### 1. Установите библиотеки Python

```bash
pip install pyserial
pip install git+https://github.com/gwy15/python_aida64
```

Источник для логики чтения памяти аиды взят от сюда: https://github.com/gwy15/python_aida64

### Шаг 2: Разрешить AIDA64 экспорт в Shared Memory
Можно задать любые параметры и вытаскивать их по именам. 
Подробнее здесь: https://www.aida64.com/user-manual/hardware-monitoring/external-applications
И здесь: https://www.aida64.com/user-manual/hardware-monitoring/external-applications/complete-sensor-value-list


## 📜 `main.py` (на ПК)
Скрипт:

- Подключается к общей памяти AIDA64
- Извлекает температуру CPU/GPU, загрузку, VRAM, RAM
- Отправляет данные на Arduino по COM-порту каждые 1 сек

Убедитесь, что в `main.py` указан **корректный COM-порт**:

```python
serial.Serial("COM3", 9600)  # ← замените COM3 на свой порт!
```

---

## 🔌 Загрузка скетча на Arduino

### Шаг 1: Установи библиотеки

Через Arduino IDE:
- **Adafruit SSD1306**
- **Adafruit GFX**

## ⚙️ Формат данных между ПК и Arduino

ПК отправляет строки в таком виде (раз в секунду):

```
CPU:52
CPUL:15
GPU:43
GPUL:25
VRAM:8142/12128
RAM:16328/48392
```

## ✅ Результат

На OLED-дисплее:

```
CPU T: 52 C L: 15%

GPU T: 43 C L: 25%
VRAM 8142/12128

RAM: 16328/48392
```

---

## Дополнительно:
- Если AIDA64 не запущен — Python скрипт не сможет прочитать общую память.
- Рекомендуется сначала залить скрипт на ардуино и только потом запускать пайтон скрипт. При отладке делать точно также.

