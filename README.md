# 🌱🤖 KAZTHOR FARM V3.5

<div align="center">

## Smart IoT Agriculture Platform

**ESP32-S3 • ESP-IDF • FreeRTOS • MQTT • Web Dashboard • Smart Automation**

An open-source smart agriculture platform designed for environmental monitoring, automation, and future AI integration.

Developed by **KAZTHOR LABS**

</div>

---

## 📌 Overview

**KAZTHOR FARM** is an open-source embedded systems project focused on building a modern smart agriculture platform using the **ESP32-S3**.

The project combines environmental sensing, IoT communication, automation, and a real-time web dashboard into a modular architecture designed for future expansion with artificial intelligence, computer vision, and distributed monitoring.

Unlike a traditional hydroponic controller, KAZTHOR FARM is intended to become a complete research and development platform for embedded agriculture technologies.

---

## ✨ What's New in Version 3.5

Version **3.5** introduces major improvements in mechanical design, electrical reliability, and user interaction.

### 🚀 New Features

- SG90 servo motor for automatic fan positioning.
- Manual fan positioning from the dashboard.
- Automatic fan positioning mode.
- 680µF power stabilization capacitor.
- 1N4007 flyback protection diode.
- Improved enclosure cable management.
- Optimized OLED display location.
- Enhanced dashboard responsiveness.
- Improved relay stability.
- Updated validation reports.
- Expanded technical documentation.

---

## 🌐 Live Dashboard

Experience the real-time dashboard directly from GitHub Pages.

🔗 https://antoniokazthor12345.github.io/Kazthorfarm/

---

## 🎥 Project Demonstration

Watch the complete project demonstration.

📺 YouTube

https://www.youtube.com/watch?v=l3PrIL1JVCo

---

# 🚀 Quick Preview

![KAZTHOR FARM Demo](media/demo/demo.gif)

---

## 🌟 Key Features

KAZTHOR FARM provides a complete embedded monitoring and automation platform including:

- 🌡️ Real-time temperature monitoring
- 🌱 Soil moisture monitoring
- 💧 Water level monitoring
- 🌬️ Automatic climate control
- 🤖 Servo-controlled fan positioning
- 💡 RGB lighting control
- 💦 Humidifier control
- 🚰 Water pump automation
- 📺 OLED monitoring display
- 🌐 MQTT communication
- 📊 Web dashboard
- 📱 Remote monitoring
- ⚡ FreeRTOS multitasking
- 📡 ESP-IDF professional firmware architecture

---

# 🎯 Project Goals

The long-term objective of KAZTHOR FARM is to create a scalable embedded platform capable of supporting modern smart agriculture technologies.

Current goals include:

- Environmental monitoring
- Intelligent automation
- Remote IoT communication
- Modular firmware architecture
- Reliable embedded hardware
- Future AI integration
- Computer vision
- Predictive agriculture

---

# 🔄 Project Evolution

## ESP32 DevKit → ESP32-S3

The project originally started using a conventional ESP32 DevKit board.

As the project evolved, additional processing power, memory, and future scalability became necessary.

Migration to the **ESP32-S3 WROOM N16R8** provided:

- Native USB support
- Dual-Core architecture
- 16 MB Flash
- 8 MB PSRAM
- Improved processing performance
- Better support for ESP-IDF
- Future camera integration
- AI-ready hardware platform

Today, KAZTHOR FARM is built entirely around the ESP-IDF ecosystem, providing a more robust architecture for future development.

---

# 🚦 System Operating States

KAZTHOR FARM continuously evaluates environmental conditions and automatically adjusts the climate controller based on three operating modes.

---

# 🟢 NORMAL

### Conditions

```txt
Temperature < 35°C

Soil Moisture >= 40%
```

### System Behavior

- Green status LED ON.
- Normal environmental monitoring.
- Automatic climate regulation.
- Dashboard reports NORMAL status.
- No alarms generated.

---

# 🟡 ALERT

### Conditions

```txt
Soil Moisture < 40%
```

### System Behavior

- Preventive climate adjustments.
- Increased monitoring frequency.
- Dashboard warning indicators.
- Automatic corrective actions when required.

---

# 🔴 CRITICAL

### Conditions

```txt
Temperature >= 35°C

OR

Soil Moisture < 20%
```

### System Behavior

- Fan Level 3 activated.
- Mist Level 3 activated.
- Water pump relay enabled.
- Red status LED ON.
- Audible buzzer alarm.
- Dashboard displays CRITICAL status.
- MQTT alert notification.

---

# 🔌 Hardware Connections

The following table summarizes the primary hardware connections used in KAZTHOR FARM V3.5.

| Device | GPIO |
|---------|------|
| OLED SDA | GPIO 8 |
| OLED SCL | GPIO 9 |
| DS18B20 | GPIO 13 |
| Soil Moisture Sensor | GPIO 4 |
| Water Pump Relay | GPIO 15 |
| Green LED | GPIO 10 |
| Red LED | GPIO 11 |
| Buzzer | GPIO 12 |
| Servo SG90 | *(Configured in firmware)* |

**DS18B20 Pull-up Resistor**

```txt
4.7kΩ between DATA and 3.3V
```

---

# 📁 Repository Structure

```text
KAZTHOR-FARM

│
├── firmware/
│   └── esp32-s3/
│
├── dashboard/
│   ├── css/
│   ├── js/
│   ├── images/
│   └── index.html
│
├── docs/
│
├── media/
│   ├── hardware/
│   ├── dashboard/
│   ├── enclosure/
│   └── demo/
│
├── validation/
│   ├── hardware/
│   ├── mqtt/
│   ├── dashboard/
│   └── functional/
│
├── README.md
├── CHANGELOG.md
├── partitions.csv
└── LICENSE
```

---

# 📸 Project Gallery

The repository includes technical documentation and validation material for every project stage.

### Hardware

- Complete hardware photographs.
- Internal enclosure layout.
- Cable management.
- Servo integration.
- Cooling system.
- Relay protection improvements.

### Dashboard

- Web dashboard screenshots.
- Real-time monitoring.
- Manual controls.
- MQTT communication.
- System status indicators.

### Documentation

- Wiring diagrams.
- Validation reports.
- Functional testing.
- Hardware verification.
- MQTT testing.
- Development logs.

---

# 🛣️ Development Roadmap

## ✅ Version 3.5

Current Release

### Hardware

- ESP32-S3 WROOM N16R8
- Servo-assisted fan positioning
- Relay-controlled water pump
- Power stabilization capacitor
- Flyback protection diode
- Improved enclosure layout

### Software

- ESP-IDF
- FreeRTOS
- MQTT
- Web Dashboard
- Automatic Climate Control
- OLED Interface

### Features

- Real-time monitoring
- Manual dashboard controls
- Automatic fan positioning
- Climate automation
- IoT connectivity

---

# 🚀 Version 4

## Smart Greenhouse

Planned features include:

- Mini greenhouse prototype.
- Real plant cultivation.
- Environmental validation.
- Long-term monitoring.
- Climate optimization.
- Historical data logging.
- Improved dashboard.
- Automatic notifications.

---

# 🌿 Version 4.5

## Hydroponic Expansion

Planned improvements:

- Hydroponic nutrient monitoring.
- Water quality analysis.
- Additional environmental sensors.
- Improved irrigation management.

---

# 🤖 Version 5

## Computer Vision & AI

Future development:

- ESP32-S3 Camera integration.
- Plant image capture.
- Computer vision.
- AI-assisted monitoring.
- Disease detection.
- Plant growth analysis.

---

# 📡 Version 6

## Distributed Agriculture

- LoRa communication.
- Multi-node deployment.
- Outdoor sensor stations.
- Remote greenhouse monitoring.

---

# 🛠 Version 7

## Professional Hardware

- Custom PCB.
- Professional enclosure.
- Industrial connectors.
- Optimized power distribution.
- Simplified wiring.

---

# 🧠 Version 8

## Intelligent Agriculture Platform

- Predictive analytics.
- Machine learning.
- Historical database.
- Advanced dashboards.
- Smart irrigation recommendations.
- AI-powered environmental optimization.

---

# 🔮 Future Development

KAZTHOR FARM is designed as a long-term research and development platform. Future releases will continue expanding its capabilities in embedded systems, IoT, automation, and intelligent agriculture.

## Short-Term Goals

- Design and manufacture a custom PCB.
- Replace the development breadboard with a production-ready design.
- Develop a custom climate control module.
- Improve power management and electrical protection.
- Optimize enclosure design.
- Integrate additional environmental sensors.

---

## Mid-Term Goals

- Modular hardware architecture.
- Advanced irrigation strategies.
- Support for hydroponic systems.
- 3D printed mechanical components.
- Multi-node communication using LoRa.
- Distributed greenhouse monitoring.
- Historical data storage.

---

## Long-Term Vision

- Computer vision using ESP32-S3 Camera.
- Artificial Intelligence assisted monitoring.
- Predictive irrigation algorithms.
- Plant disease detection.
- Growth analysis.
- Cloud data analytics.
- Smart greenhouse deployment.
- Commercial-grade embedded platform.

---

# 🌍 Project Vision

KAZTHOR FARM aims to become a complete **Smart Agriculture Platform**, combining embedded systems, IoT, automation, cloud connectivity, and Artificial Intelligence into a single modular ecosystem.

The project is continuously evolving with a strong focus on:

- Embedded Systems
- Internet of Things (IoT)
- Smart Automation
- Environmental Monitoring
- Cloud Connectivity
- Computer Vision
- Artificial Intelligence
- Open Source Development

---

# 👨‍💻 Author

## Antonio Castor Silva

**Founder — KAZTHOR LABS**

Building practical embedded systems, IoT solutions, automation platforms, and smart agriculture technologies using modern open-source tools.

### Technologies

- ESP32-S3
- ESP-IDF
- FreeRTOS
- MQTT
- HiveMQ Cloud
- HTML
- CSS
- JavaScript
- GitHub Pages

---

## 🌐 Links

### GitHub

https://github.com/antoniokazthor12345

### Live Dashboard

https://antoniokazthor12345.github.io/Kazthorfarm/

### YouTube

https://www.youtube.com/@AntonioKazthor

---

# 🤝 Contributing

Contributions, suggestions, bug reports, and feature requests are always welcome.

If you would like to contribute to KAZTHOR FARM, feel free to open an Issue or submit a Pull Request.

Every contribution helps improve the project and supports the development of open-source smart agriculture technologies.

---

# 📄 License

This project is released under the **MIT License**.

You are free to use, modify, distribute, and build upon this project while preserving the original license and copyright notice.

See the **LICENSE** file for additional information.

---

# ⭐ Support the Project

If you found this project useful or inspiring, please consider:

- ⭐ Starring this repository.
- 🍴 Forking the project.
- 🐛 Reporting bugs or suggesting improvements.
- 📢 Sharing the project with the embedded systems and IoT community.
- ▶️ Following future developments from **KAZTHOR LABS**.

Your support helps drive the continued development of open-source embedded technologies, smart agriculture solutions, and educational IoT projects.

---

<div align="center">

## 🌱 KAZTHOR FARM V3.5

**Smart Agriculture • Embedded Systems • IoT • Automation • ESP-IDF • FreeRTOS**

Developed with ❤️ by **KAZTHOR LABS**

*"Building the next generation of open-source smart agriculture platforms."*

</div>