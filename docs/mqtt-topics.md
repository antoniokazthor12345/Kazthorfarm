# 🌐 MQTT Topics

# 🌱 KAZTHOR FARM V3.5

**MQTT Communication Documentation**

This document describes the MQTT communication architecture used by **KAZTHOR FARM V3.5**.

---

# 📡 MQTT Broker

## HiveMQ Cloud

Communication is handled using **HiveMQ Cloud**.

### Connection Type

```txt
Secure MQTT
MQTT over WebSockets
TLS Encryption
```

### Used By

- ESP32-S3 Firmware
- Web Dashboard
- Remote Monitoring System

---

# 📂 Base Topic

```txt
kazthor/farm01
```

All MQTT topics are organized from this base topic.

---

# 📤 Publish Topics

## Sensor Data

```txt
kazthor/farm01/data
```

Published by

```txt
ESP32-S3
```

Consumed by

```txt
Dashboard
```

### Example Payload

```json
{
  "temp": 24.75,
  "soil": 68,
  "water": 55,
  "state": "NORMAL",
  "pump": false,
  "relay": false,
  "fan": 2,
  "mist": 1,
  "servo": 90,
  "light": false
}
```

---

## System Status

```txt
kazthor/farm01/status
```

### Example Payload

```json
{
  "device": "kazthor-farm-v35",
  "version": "3.5",
  "online": true,
  "wifi": true,
  "mqtt": true,
  "uptime": 2540
}
```

---

## Alerts

```txt
kazthor/farm01/alerts
```

### Example Payload

```json
{
  "level": "CRITICAL",
  "message": "High temperature detected",
  "temp": 36.2,
  "soil": 18,
  "water": 42
}
```

---

## Heartbeat

```txt
kazthor/farm01/heartbeat
```

Published periodically to indicate that the ESP32-S3 is still connected.

Example

```json
{
    "alive": true,
    "timestamp": 1722470200
}
```

---

# 📥 Subscribe Topics

## Commands

```txt
kazthor/farm01/cmd
```

Subscribed by

```txt
ESP32-S3
```

Published by

```txt
Dashboard
```

---

# 🎛️ Command Payloads

## Water Pump

```json
{
    "relay": true
}
```

```json
{
    "relay": false
}
```

---

## Fan Speed

```json
{
    "fan": 3
}
```

Valid Values

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

---

## Humidifier

```json
{
    "mist": 2
}
```

Valid Values

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

---

## RGB Light

```json
{
    "light": true
}
```

```json
{
    "light": false
}
```

---

## Servo Position

Manual positioning.

```json
{
    "servo": 0
}
```

```json
{
    "servo": 45
}
```

```json
{
    "servo": 90
}
```

```json
{
    "servo": 135
}
```

```json
{
    "servo": 180
}
```

The servo can also be controlled using the Dashboard UP and DOWN buttons when the cooling fan is OFF.

---

## Operating Mode

```json
{
    "mode": "AUTO"
}
```

```json
{
    "mode": "MANUAL"
}
```

---

# 🚦 System States

## NORMAL

```json
{
    "state":"NORMAL"
}
```

Meaning

- Environmental conditions are within acceptable limits.
- No automatic actions required.

---

## ALERT

```json
{
    "state":"ALERT"
}
```

Meaning

- Soil moisture is below the recommended threshold.
- Climate control may activate automatically.

---

## CRITICAL

```json
{
    "state":"CRITICAL"
}
```

Meaning

- High temperature detected.
- Soil moisture critically low.
- Automatic emergency actions enabled.
- Audible alarm activated.

---

# 🔁 MQTT Data Flow

```txt
                 Sensors
                     │
                     ▼
               ESP32-S3 Firmware
                     │
      ┌──────────────┼───────────────┐
      │              │               │
      ▼              ▼               ▼

   Sensor Data     Status         Alerts
      │              │               │
      └──────────────┼───────────────┘
                     ▼

             HiveMQ Cloud Broker

                     ▲

      ┌──────────────┼──────────────┐
      │              │              │

 Dashboard      Mobile Apps     MQTT Clients

      │
      ▼

Publish Commands

      │

      ▼

ESP32-S3

      │

      ▼

Pump
Fan
Humidifier
Servo
OLED
Relay
```

---

# 📊 Dashboard Features

The Dashboard can

- Display temperature
- Display soil moisture
- Display water level
- Display current system state
- Display fan level
- Display humidifier level
- Display relay state
- Display servo position
- Control actuators in real time
- Send MQTT commands
- Receive live telemetry

---

# 🧪 Testing

The project can be tested using

- MQTTX
- HiveMQ Web Client
- Mosquitto CLI

Example Subscription

```txt
kazthor/farm01/data
```

Example Publish

```json
{
    "fan":1
}
```

---

# 🔐 Security

Never upload

```txt
WiFi SSID
WiFi Password
MQTT Username
MQTT Password
Broker URL
Certificates
API Keys
```

Recommended

```txt
Store credentials in configuration files
ignored by Git using .gitignore.
```

---

# ✅ MQTT Checklist

Before publishing a new release verify

- MQTT credentials removed
- Broker credentials hidden
- Dashboard topics updated
- Firmware topics updated
- JSON payloads synchronized
- Dashboard reconnect works
- ESP32 reconnect works
- Heartbeat active
- Servo commands working
- Automatic mode tested
- Manual mode tested

---

# 🚀 Future MQTT Features

## V4

- Greenhouse telemetry
- Plant monitoring

## V4.5

- Hydroponic telemetry
- Water quality monitoring
- Reservoir analytics

## V5

- Camera status
- AI inference results
- Plant health reports

## V6

- LoRa Gateway
- Remote MQTT Bridge

## V7

- Multi-node architecture
- STM32 communication

## V8

- Robot telemetry
- Drone telemetry
- Autonomous mission control

---

# 👨‍💻 Author

**Antonio Castor Silva**

Founder of **KAZTHOR LABS**

Embedded Systems • ESP32 • ESP-IDF • FreeRTOS • MQTT • IoT • Smart Farming