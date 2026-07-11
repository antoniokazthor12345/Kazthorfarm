# 🔌 Wiring Guide

# KAZTHOR FARM V1

This document describes the wiring configuration used in KAZTHOR FARM V1.

---

# 📋 Pin Assignment Table

| Device       | ESP32-S3 GPIO |
| ------------ | ------------- |
| OLED SDA     | GPIO 8        |
| OLED SCL     | GPIO 9        |
| Green LED    | GPIO 10       |
| Red LED      | GPIO 11       |
| Buzzer       | GPIO 12       |
| DS18B20      | GPIO 13       |
| Soil Sensor  | GPIO 4        |
| Water Sensor | ADC Input     |
| Relay Module | GPIO 15       |
| FAN PC817    | GPIO 16       |
| MIST PC817   | GPIO 17       |
| LIGHT PC817  | GPIO 18       |
| TIMER PC817  | GPIO 19       |

---

# 🖥 OLED Display

SSD1306 128x64

Connection:

```txt
OLED          ESP32-S3

VCC     --->  3.3V
GND     --->  GND
SDA     --->  GPIO8
SCL     --->  GPIO9
```

I2C Address:

```txt
0x3C
```

---

# 🌡 DS18B20

Connection:

```txt
DS18B20       ESP32-S3

RED      ---> 3.3V
BLACK    ---> GND
YELLOW   ---> GPIO13
```

Required Pull-up Resistor:

```txt
4.7kΩ

GPIO13 ---- 4.7kΩ ---- 3.3V
```

---

# 🌱 Soil Moisture Sensor

Connection:

```txt
Sensor         ESP32-S3

VCC      ---> 3.3V
GND      ---> GND
AO       ---> GPIO4
```

Reading Type:

```txt
Analog
```

---

# 💧 Water Level Sensor

Connection:

```txt
Sensor         ESP32-S3

VCC      ---> 3.3V
GND      ---> GND
AO       ---> ADC Input
```

Reading Type:

```txt
Analog
```

---

# 🟢 Green LED

Connection:

```txt
GPIO10
  │
220Ω
  │
 LED
  │
 GND
```

Purpose:

```txt
NORMAL state indicator
```

---

# 🔴 Red LED

Connection:

```txt
GPIO11
  │
220Ω
  │
 LED
  │
 GND
```

Purpose:

```txt
ALERT / CRITICAL indicator
```

---

# 🚨 Buzzer

Connection:

```txt
GPIO12 ---- Buzzer ---- GND
```

Purpose:

```txt
Alarm notification
```

---

# ⚡ Relay Module

Control Side:

```txt
Relay Module      ESP32-S3

S          ---> GPIO15
VCC        ---> 5V
GND        ---> GND
```

Power Side:

```txt
5V Supply +
       │
       ▼

      COM

      NO
       │

Fan Positive
```

Fan Negative:

```txt
Direct to GND
```

Operation:

```txt
Relay OFF
    ↓
No power to Fan

Relay ON
    ↓
Fan receives power
```

---

# 🔌 PC817 Optocouplers

Each climate-control button uses an independent PC817.

---

## FAN Control

```txt
ESP32 GPIO16
      │
     1kΩ
      │
 PC817 Input
```

Output side connected across FAN button contacts.

---

## MIST Control

```txt
ESP32 GPIO17
      │
     1kΩ
      │
 PC817 Input
```

Output side connected across MIST button contacts.

---

## LIGHT Control

```txt
ESP32 GPIO18
      │
     1kΩ
      │
 PC817 Input
```

Output side connected across LIGHT button contacts.

---

## TIMER Control

```txt
ESP32 GPIO19
      │
     1kΩ
      │
 PC817 Input
```

Output side connected across TIMER button contacts.

---

# 🌬 Climate Controller Wiring

USB-C Powered Device

Features:

```txt
FAN
MIST
RGB LIGHT
TIMER
```

Control Method:

```txt
Button Emulation
using PC817 optocouplers
```

The ESP32 does not directly drive the motors.

Instead, it simulates button presses through isolated optocouplers.

---

# 📊 Complete Wiring Diagram

```txt
                  ┌─────────────────┐
                  │    ESP32-S3     │
                  └────────┬────────┘
                           │
      ┌────────────────────┼────────────────────┐
      │                    │                    │

      ▼                    ▼                    ▼

  DS18B20           Soil Sensor        Water Sensor

      │
      ▼

   OLED SSD1306

      │
      ▼

 Relay Module

      │
      ▼

 Climate Controller

      │
      ▼

 FAN / MIST / LIGHT

      ▲
      │

 PC817 Optocouplers

      ▲
      │

 ESP32 GPIO16-19
```

---

# ⚠ Notes

* All grounds must be shared.
* Relay power should use a stable 5V source.
* Use 4.7kΩ pull-up resistor on DS18B20.
* Use 220Ω resistors on LEDs.
* Use 1kΩ resistors on PC817 inputs.
* Verify GPIO assignments before flashing firmware.

---

Author:

Antonio Castor Silva

KAZTHOR LABS
