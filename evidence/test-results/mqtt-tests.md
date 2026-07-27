# 🌐 MQTT Validation Report

# KAZTHOR FARM V3.5

**Hardware Refinement Update**

---

# 📡 MQTT Broker

Broker

HiveMQ Cloud

Protocol

MQTT over TLS

Port

8883

Encryption

TLS

Status

✅ PASS

---

# 📶 WiFi Connection

WiFi Connection

PASS

IP Address Assigned

PASS

Signal Strength

PASS

Automatic Reconnection

PASS

---

# 🔗 Broker Connection

Broker Authentication

PASS

TLS Handshake

PASS

MQTT Session Created

PASS

Keep Alive

PASS

Connection Stability

PASS

---

# 📤 Publish Tests

Telemetry Data

PASS

Temperature

PASS

Soil Moisture

PASS

Water Level

PASS

System State

PASS

Fan Status

PASS

Humidifier Status

PASS

Servo Position

PASS

Relay Status

PASS

Alerts

PASS

Heartbeat

PASS

---

# 📥 Subscribe Tests

Command Reception

PASS

Fan Control

PASS

Humidifier Control

PASS

Relay Control

PASS

Servo Control

PASS

RGB Light Control

PASS

Timer Control

PASS

Automatic Mode

PASS

Manual Mode

PASS

---

# 🔄 Reconnection Tests

WiFi Recovery

PASS

MQTT Recovery

PASS

Automatic Resubscription

PASS

Telemetry Recovery

PASS

Heartbeat Recovery

PASS

---

# 📊 Dashboard Communication

Real-Time Data

PASS

Sensor Synchronization

PASS

System Status Updates

PASS

Alert Notifications

PASS

Manual Commands

PASS

---

# 🔒 Security Tests

TLS Encryption

PASS

Broker Authentication

PASS

Secure Session

PASS

Credentials Validation

PASS

Unauthorized Access

BLOCKED

---

# ⚡ Performance Tests

Average Latency

LOW

Packet Loss

0%

Connection Stability

PASS

Reconnect Attempts

0

Communication Errors

0

---

# 📋 MQTT Topics Validation

Published Topics

✅ kazthor/farm01/data

✅ kazthor/farm01/status

✅ kazthor/farm01/alerts

✅ kazthor/farm01/heartbeat

Subscribed Topics

✅ kazthor/farm01/cmd

Result

PASS

---

# 🏆 Overall MQTT Validation

| Test | Result |
|------|--------|
| WiFi Connection | ✅ PASS |
| Broker Connection | ✅ PASS |
| TLS Security | ✅ PASS |
| Publish | ✅ PASS |
| Subscribe | ✅ PASS |
| Dashboard Communication | ✅ PASS |
| Reconnection | ✅ PASS |
| Heartbeat | ✅ PASS |
| Telemetry | ✅ PASS |
| Remote Commands | ✅ PASS |

---

# ✅ Final Result

**MQTT communication has been successfully validated.**

The system maintains a secure, stable, and reliable connection with HiveMQ Cloud, supporting real-time telemetry, dashboard synchronization, remote commands, and automatic recovery after connection interruptions.

**Overall MQTT Status**

✅ **PASS**

---

**KAZTHOR LABS**

**Antonio Castor Silva**

*ESP32 • ESP-IDF • FreeRTOS • MQTT • IoT • Smart Farming • Embedded Systems*