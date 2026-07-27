# 🔧 Hardware Validation Report

# KAZTHOR FARM V3.5

**Hardware Refinement Update**

---

# 🧠 Main Controller

## ESP32-S3 WROOM

Status:

✅ PASS

Validation:

- Boot successful
- FreeRTOS running
- Stable operation
- WiFi connected
- MQTT connected

---

# 🖥 OLED Display

## SSD1306 128x64

Status:

✅ PASS

Validation:

- Display initialized
- Real-time sensor data
- System state visualization
- Fan and humidifier status
- Stable communication (I2C)

---

# 🌡 Temperature Sensor

## DS18B20

Status:

✅ PASS

Validation:

- Sensor detected
- Accurate temperature readings
- Stable 1-Wire communication

---

# 🌱 Soil Moisture Sensor

Status:

✅ PASS

Validation:

- Analog readings
- Correct calibration
- Automatic state detection

---

# 💧 Water Level Sensor

Status:

✅ PASS

Validation:

- Analog readings
- Reservoir monitoring
- Critical level detection

---

# ⚡ Relay Module

Status:

✅ PASS

Validation:

- Water pump ON/OFF control
- Stable switching
- No unexpected activation

---

# 🔌 PC817 Optocouplers

Status:

✅ PASS

Validation:

- FAN button emulation
- MIST button emulation
- LIGHT button emulation
- TIMER button emulation
- Electrical isolation verified

---

# 🔄 Servo Motor

Status:

✅ PASS

Validation:

- Automatic positioning
- Manual UP/DOWN control
- Smooth movement
- Correct alignment with cooling fan

---

# 🚿 Water Pump

Status:

✅ PASS

Validation:

- Relay activation
- Stable operation
- Proper response to automatic control

---

# 🛡 Protection Diode

## 1N4007

Status:

✅ PASS

Validation:

- Flyback protection verified
- Voltage spike suppression
- Relay protection confirmed

---

# ⚡ Power Stabilization

## Electrolytic Capacitor

Specifications:

680µF / 16V

Status:

✅ PASS

Validation:

- Stable servo power
- Reduced voltage fluctuations
- No ESP32 resets detected

---

# 🚨 Buzzer

Status:

✅ PASS

Validation:

- Alert mode
- Critical mode
- Audible alarm confirmed

---

# 💡 Status LEDs

Green LED

✅ PASS

Red LED

✅ PASS

Validation:

- NORMAL indication
- ALERT indication
- CRITICAL indication

---

# 🌬 Climate Controller

Status:

✅ PASS

Validation:

- Fan operation
- Humidifier operation
- RGB Light operation
- Timer operation
- Automatic climate control

---

# 📊 Hardware Validation Summary

| Component | Result |
|-----------|--------|
| ESP32-S3 WROOM | ✅ PASS |
| OLED Display | ✅ PASS |
| DS18B20 | ✅ PASS |
| Soil Moisture Sensor | ✅ PASS |
| Water Level Sensor | ✅ PASS |
| Relay Module | ✅ PASS |
| PC817 Interfaces | ✅ PASS |
| Servo Motor | ✅ PASS |
| Water Pump | ✅ PASS |
| 1N4007 Diode | ✅ PASS |
| 680µF Capacitor | ✅ PASS |
| Buzzer | ✅ PASS |
| Status LEDs | ✅ PASS |
| Climate Controller | ✅ PASS |

---

# 🏆 Final Result

**All hardware components were successfully tested and validated.**

**Overall Hardware Status**

✅ **PASS**

---

**KAZTHOR LABS**

**Antonio Castor Silva**

*ESP32 • ESP-IDF • FreeRTOS • IoT • Smart Farming • Embedded Systems*