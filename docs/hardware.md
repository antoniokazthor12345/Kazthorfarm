# 🔧 Hardware Documentation

# KAZTHOR FARM V1

This document describes all hardware components used in the KAZTHOR FARM V1 project.

---

# 🧠 Main Controller

## ESP32-S3 WROOM

The ESP32-S3 is the main controller of the system.

Features:

* Dual-core processor
* WiFi
* Bluetooth LE
* FreeRTOS support
* Multiple ADC channels
* GPIO expansion
* OTA capable
* ESP-IDF compatible
* Future camera support

Responsibilities:

* Sensor acquisition
* Climate control
* MQTT communication
* Dashboard integration
* OLED updates
* Alert management

---

# 🌡 Temperature Sensor

## DS18B20 Waterproof Sensor

Used to monitor ambient temperature.

Specifications:

* Digital 1-Wire communication
* High accuracy
* Waterproof probe
* Long cable support

Connection:

```txt
VCC  -> 3.3V
GND  -> GND
DATA -> GPIO13
```

Additional hardware:

```txt
4.7kΩ pull-up resistor
between DATA and 3.3V
```

---

# 🌱 Soil Moisture Sensor

Used to measure soil humidity levels.

Function:

* Detect dry soil
* Detect optimal humidity
* Trigger irrigation decisions

Connection:

```txt
VCC -> 3.3V
GND -> GND
AO  -> GPIO4
```

Type:

* Analog sensor

---

# 💧 Water Level Sensor

Monitors water availability inside the reservoir.

Purpose:

* Prevent dry-run conditions
* Generate alerts
* Enable safe irrigation

Connection:

```txt
VCC -> 3.3V
GND -> GND
AO  -> ADC Channel
```

Type:

* Analog sensor

---

# 🖥 OLED Display

## SSD1306

Display Size:

```txt
128 x 64
```

Communication:

```txt
I2C
```

Connection:

```txt
SDA -> GPIO8
SCL -> GPIO9
VCC -> 3.3V
GND -> GND
```

Displayed Information:

* Temperature
* Soil Moisture
* Water Level
* System State
* MQTT Status

---

# 🚨 Buzzer

Used for audible alarms.

Functions:

* Critical alerts
* System warnings
* Error notification

Connection:

```txt
Signal -> GPIO12
VCC    -> 3.3V
GND    -> GND
```

---

# 🟢🔴 Status LEDs

System indicators.

## Green LED

Meaning:

```txt
System operating normally
```

Connection:

```txt
GPIO10
```

---

## Red LED

Meaning:

```txt
Warning or Critical State
```

Connection:

```txt
GPIO11
```

---

# ⚡ Relay Module

## Single Channel Relay

Purpose:

* Complete power cutoff
* Fan shutdown
* Safety isolation

Control Connection:

```txt
S   -> ESP32 GPIO
VCC -> 5V
GND -> GND
```

Power Side:

```txt
COM
NO
NC
```

Configuration:

```txt
COM -> 5V Supply
NO  -> Fan Positive
```

The relay guarantees complete shutdown of the climate controller when not required.

---

# 🌬 Climate Controller

USB Powered Fan/Humidifier Unit

Integrated Functions:

* Fan
* Mist Generator
* RGB Light
* Timer

Power:

```txt
5V USB-C
```

Fan Levels:

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

Mist Levels:

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

---

# 🔌 Optocoupler Interface

## PC817

Used to electrically isolate the ESP32 from the climate controller electronics.

Functions Controlled:

* FAN button
* MIST button
* LIGHT button
* TIMER button

Benefits:

* Electrical isolation
* Increased reliability
* Reduced noise
* Hardware protection

---

# 📊 Hardware Architecture

```txt
                    KAZTHOR FARM V1

                   ┌───────────────┐
                   │   ESP32-S3    │
                   └───────┬───────┘
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
       ▼                   ▼                   ▼

   DS18B20           Soil Sensor      Water Sensor

       │
       ▼

   FreeRTOS Tasks

       │
       ▼

 MQTT + Dashboard

       │
       ▼

  OLED Display

       │
       ▼

  Control Logic

       │
       ▼

 Relay + PC817

       │
       ▼

 FAN / MIST / LIGHT
```

---

# 🚀 Future Hardware Upgrades

## V2

* ESP32-S3 Camera
* Remote image capture

## V3

* SD Card storage
* Historical logging

## V4

* Custom PCB
* Industrial enclosure
* Solar power support

---

Author:

Antonio Castor Silva

KAZTHOR LABS
