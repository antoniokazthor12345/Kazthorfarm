# 🌐 MQTT Topics

# KAZTHOR FARM V1

This document describes the MQTT communication used by KAZTHOR FARM V1.

---

# 📡 Broker

Cloud MQTT Broker:

```txt
HiveMQ Cloud
```

Connection type:

```txt
Secure MQTT / MQTT over WebSockets
```

Used by:

* ESP32-S3 firmware
* Web dashboard
* Remote monitoring system

---

# 🧭 Base Topic

```txt
kazthor/farm01
```

All project topics are structured from this base topic.

---

# 📤 Publish Topics

## Sensor Data

```txt
kazthor/farm01/data
```

Published by:

```txt
ESP32-S3
```

Used by:

```txt
Dashboard
```

Example payload:

```json
{
  "temp": 24.75,
  "soil": 68,
  "water": 55,
  "state": "NORMAL",
  "pump": false,
  "temp_ok": true,
  "fan": 0,
  "mist": 0,
  "relay": false
}
```

---

## System Status

```txt
kazthor/farm01/status
```

Example payload:

```json
{
  "device": "kazthor-farm-v1",
  "online": true,
  "wifi": true,
  "mqtt": true,
  "uptime": 120
}
```

---

## Alerts

```txt
kazthor/farm01/alerts
```

Example payload:

```json
{
  "level": "CRITICAL",
  "message": "High temperature or low soil moisture detected",
  "temp": 36.2,
  "soil": 18,
  "water": 42
}
```

---

# 📥 Subscribe Topics

## Commands

```txt
kazthor/farm01/cmd
```

Subscribed by:

```txt
ESP32-S3
```

Used by:

```txt
Dashboard
```

---

# 🎛️ Command Payloads

## Turn Relay ON

```json
{
  "relay": true
}
```

---

## Turn Relay OFF

```json
{
  "relay": false
}
```

---

## Set FAN Level

```json
{
  "fan": 3
}
```

Valid values:

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

---

## Set MIST Level

```json
{
  "mist": 3
}
```

Valid values:

```txt
0 = OFF
1 = LOW
2 = MEDIUM
3 = HIGH
```

---

## Turn Light ON/OFF

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

## System Mode

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
  "state": "NORMAL"
}
```

Meaning:

* Temperature is safe
* Soil humidity is acceptable
* No critical alert

---

## ALERTA

```json
{
  "state": "ALERTA"
}
```

Meaning:

* Soil moisture is below recommended level
* Climate control may be activated

---

## CRITICAL

```json
{
  "state": "CRITICAL"
}
```

Meaning:

* Temperature is too high
* Soil moisture is critically low
* Alarm actions are activated

---

# 🔁 MQTT Data Flow

```txt
ESP32-S3
   │
   ├── publishes sensor data
   │       └── kazthor/farm01/data
   │
   ├── publishes system status
   │       └── kazthor/farm01/status
   │
   ├── publishes alerts
   │       └── kazthor/farm01/alerts
   │
   ▼

HiveMQ Cloud Broker

   ▲
   │
   └── Dashboard subscribes to:
           kazthor/farm01/data
           kazthor/farm01/status
           kazthor/farm01/alerts


Dashboard
   │
   └── publishes commands to:
           kazthor/farm01/cmd

ESP32-S3
   │
   └── receives commands and controls:
           Relay
           FAN
           MIST
           LIGHT
```

---

# 🧪 Testing with MQTT Client

You can test the project using any MQTT client.

Recommended clients:

* MQTTX
* HiveMQ Web Client
* Mosquitto CLI

Example subscribe:

```txt
kazthor/farm01/data
```

Example publish command:

```json
{
  "fan": 1
}
```

---

# 🔐 Security Notes

Do not upload real broker credentials to GitHub.

Avoid publishing:

```txt
WiFi SSID
WiFi password
MQTT username
MQTT password
Broker URL
```

Recommended approach:

```txt
Use config files excluded by .gitignore
or placeholder values inside example files.
```

---

# ✅ MQTT Checklist

Before publishing the repository, verify:

* MQTT credentials are hidden.
* Topics match firmware code.
* Topics match dashboard code.
* Payload keys are consistent.
* Dashboard reconnects if broker disconnects.
* ESP32 reconnects if WiFi or MQTT fails.

---

Author:

Antonio Castor Silva

KAZTHOR LABS
