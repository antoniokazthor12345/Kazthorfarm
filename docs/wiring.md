# 🔌 Wiring Guide

# 🌱 KAZTHOR FARM V3.5

**Hardware Wiring Documentation**

This document describes the wiring configuration used in **KAZTHOR FARM V3.5**.

---

# 📋 ESP32-S3 Pin Assignment

| Device | ESP32-S3 GPIO |
|----------|---------------|
| OLED SDA | GPIO 8 |
| OLED SCL | GPIO 9 |
| Green LED | GPIO 10 |
| Red LED | GPIO 11 |
| Buzzer | GPIO 12 |
| DS18B20 | GPIO 13 |
| Soil Moisture Sensor | GPIO 4 (ADC) |
| Water Level Sensor | ADC Input |
| Relay Module | GPIO 15 |
| FAN PC817 | GPIO 16 |
| MIST PC817 | GPIO 17 |
| LIGHT PC817 | GPIO 18 |
| TIMER PC817 | GPIO 19 |
| Servo Signal | GPIO 20 *(Adjust if different)* |

---

# 🖥 OLED Display

## SSD1306 (128x64)

### Connection

```txt
OLED               ESP32-S3

VCC      --------> 3.3V
GND      --------> GND
SDA      --------> GPIO8
SCL      --------> GPIO9
```

I2C Address

```txt
0x3C
```

---

# 🌡 DS18B20 Temperature Sensor

### Connection

```txt
DS18B20            ESP32-S3

RED      --------> 3.3V
BLACK    --------> GND
YELLOW   --------> GPIO13
```

Required Pull-up Resistor

```txt
GPIO13 ----- 4.7kΩ ----- 3.3V
```

---

# 🌱 Soil Moisture Sensor

### Connection

```txt
Sensor             ESP32-S3

VCC      --------> 3.3V
GND      --------> GND
AO       --------> GPIO4
```

Reading Type

```txt
Analog ADC
```

---

# 💧 Water Level Sensor

### Connection

```txt
Sensor             ESP32-S3

VCC      --------> 3.3V
GND      --------> GND
AO       --------> ADC Input
```

Reading Type

```txt
Analog ADC
```

---

# 🟢 Green Status LED

```txt
GPIO10
   │
220Ω
   │
 LED
   │
 GND
```

Purpose

```txt
System Normal
```

---

# 🔴 Red Status LED

```txt
GPIO11
   │
220Ω
   │
 LED
   │
 GND
```

Purpose

```txt
ALERT / CRITICAL
```

---

# 🚨 Buzzer

```txt
GPIO12 ------ Buzzer ------ GND
```

Purpose

```txt
Alarm Notification
```

---

# ⚡ Relay Module

Controls the Water Pump.

### Control Side

```txt
Relay             ESP32-S3

Signal  --------> GPIO15
VCC     --------> 5V
GND     --------> GND
```

### Load Side

```txt
5V Supply
    │
   COM
    │
   NO
    │
Water Pump (+)
```

Water Pump (-)

```txt
Direct to GND
```

---

# 🛡 Flyback Protection Diode

## 1N4007

Installed directly across the water pump terminals.

```txt
        +5V
         │
         │
      Water Pump
         │
         │
        GND

       ┌───────┐
       │ 1N4007│
       └───────┘
```

Purpose

- Protect relay contacts
- Suppress voltage spikes
- Increase hardware reliability

---

# 🔄 Servo Motor

## SG90 Micro Servo

Controls the airflow direction.

### Connection

```txt
Servo              ESP32-S3

VCC      --------> 5V
GND      --------> GND
Signal   --------> GPIO20
```

### Operating Modes

Automatic

```txt
Moves automatically when
the cooling fan is active.
```

Manual

```txt
Controlled from the
Dashboard UP/DOWN buttons
when the fan is OFF.
```

---

# ⚡ Power Stabilization Capacitor

## Electrolytic Capacitor

Specifications

```txt
680µF
16V
```

Installation

```txt
5V
 │
 │
680µF
 │
 │
GND
```

Purpose

- Servo voltage stabilization
- Prevent voltage drops
- Reduce electrical noise
- Improve ESP32 stability

---

# 🔌 PC817 Optocouplers

The ESP32 controls the climate controller through isolated PC817 optocouplers.

---

## FAN Button

```txt
ESP32 GPIO16
      │
     1kΩ
      │
 PC817 Input
```

---

## MIST Button

```txt
ESP32 GPIO17
      │
     1kΩ
      │
 PC817 Input
```

---

## LIGHT Button

```txt
ESP32 GPIO18
      │
     1kΩ
      │
 PC817 Input
```

---

## TIMER Button

```txt
ESP32 GPIO19
      │
     1kΩ
      │
 PC817 Input
```

Output side connected directly across each push-button on the climate controller.

---

# 🌬 Climate Controller

USB-C Powered Device

Integrated Functions

```txt
Cooling Fan

Humidifier

RGB Light

Timer
```

Control Method

```txt
Button Emulation

Using PC817 Optocouplers
```

The ESP32 never drives the motors directly.

Instead, it simulates button presses while maintaining electrical isolation.

---

# 📊 Complete Wiring Overview

```txt
                         KAZTHOR FARM V3.5

                     ┌────────────────────┐
                     │     ESP32-S3       │
                     └─────────┬──────────┘
                               │
      ┌──────────────┬──────────┼───────────────┬──────────────┐
      │              │          │               │              │
      ▼              ▼          ▼               ▼              ▼

  DS18B20      Soil Sensor  Water Sensor     OLED         Buzzer

                               │
                               ▼

                         FreeRTOS Tasks

                               │
                               ▼

                          MQTT Broker

                               │
                               ▼

                        Web Dashboard

                               │
                               ▼

                      Climate Controller

          ┌────────────┬────────────┬────────────┬────────────┐
          │            │            │            │
          ▼            ▼            ▼            ▼

        Relay       PC817        Servo      OLED Display

          │
          ▼

     Water Pump
          │
      1N4007 Diode
          │
      680µF Capacitor
```

---

# ⚠ Electrical Notes

- All grounds must be connected together.
- Use a stable 5V power supply for the relay and servo.
- Install a 4.7kΩ pull-up resistor on the DS18B20.
- Use 220Ω resistors for both LEDs.
- Use 1kΩ resistors on every PC817 input.
- Install the 1N4007 diode directly across the water pump terminals.
- Install the 680µF / 16V capacitor near the servo power supply.
- Verify GPIO assignments before flashing the firmware.

---

# ✅ Wiring Checklist

Before powering the system verify:

- OLED connected correctly
- DS18B20 pull-up resistor installed
- Relay powered with stable 5V
- Servo powered correctly
- Capacitor installed
- Flyback diode installed
- PC817 wiring verified
- Shared GND
- MQTT communication tested
- Dashboard control verified

---

# 👨‍💻 Author

**Antonio Castor Silva**

Founder of **KAZTHOR LABS**

Embedded Systems • ESP-IDF • FreeRTOS • IoT • Smart Farming • Automation