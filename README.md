# 🌱🤖 KAZTHOR FARM V3

## Smart IoT Hydroponic Automation System

Sistema inteligente de monitoreo y automatización para cultivos hidropónicos utilizando **ESP32-S3, ESP-IDF, FreeRTOS, MQTT y Dashboard Web en tiempo real**.

KAZTHOR FARM V3 forma parte del ecosistema **KAZTHOR LABS**, una plataforma de desarrollo enfocada en sistemas embebidos, Internet de las Cosas (IoT), automatización e integración hardware/software.

---

# 🎥 Project Demo

Video completo de demostración:

▶️ YouTube:
https://www.youtube.com/watch?v=l3PrIL1JVCo

---

# 🚀 Quick Preview

![KAZTHOR FARM V3 Demo](media/demo/demo.gif)

El sistema permite:

* Monitoreo de temperatura en tiempo real.
* Lectura de humedad del suelo.
* Supervisión del nivel de agua.
* Control automático de ventilación.
* Control de humidificación.
* Control de iluminación.
* Sistema de alarmas.
* Comunicación IoT mediante MQTT.
* Control remoto mediante dashboard web.

---

# 📖 Overview

KAZTHOR FARM V3 es un sistema IoT diseñado para automatizar y supervisar un cultivo hidropónico doméstico.

El objetivo principal del proyecto es crear una plataforma capaz de:

* Obtener información ambiental mediante sensores.
* Procesar datos localmente.
* Tomar decisiones automáticas.
* Controlar dispositivos físicos.
* Mostrar información en una interfaz web.
* Mantener comunicación bidireccional entre hardware y software.

El proyecto evolucionó desde un prototipo basado en ESP32 DevKit hasta una arquitectura más robusta utilizando ESP32-S3 y ESP-IDF.

---

# 🎯 Project Goals

Los principales objetivos del proyecto son:

* Crear un sistema de agricultura inteligente basado en IoT.
* Automatizar procesos que normalmente requieren supervisión manual.
* Desarrollar una arquitectura escalable.
* Implementar comunicación en tiempo real.
* Integrar sensores, actuadores y software web.
* Crear una base para futuras versiones con inteligencia artificial y visión por computadora.

---

# 🔄 Project Evolution

## ESP32 DevKit V1 → ESP32-S3

La primera versión del proyecto fue desarrollada utilizando un ESP32 DevKit V1.

Con esta plataforma fue posible:

* Crear el primer prototipo.
* Validar sensores.
* Desarrollar el dashboard.
* Implementar comunicación MQTT.

Posteriormente el proyecto migró al ESP32-S3 debido a la necesidad de contar con mayores capacidades para futuras expansiones.

El ESP32-S3 ofrece:

* Mayor memoria.
* Mejor capacidad de procesamiento.
* USB nativo.
* Mayor posibilidad de integración con cámaras.
* Preparación para visión por computadora e inteligencia artificial.

La migración permitió construir una arquitectura más preparada para futuras versiones.

---

# ⚙️ Hardware

## Main Controller

### ESP32-S3 WROOM N16R8

Características principales:

* Dual Core Processor.
* WiFi integrado.
* Bluetooth.
* 8MB PSRAM.
* Compatible con ESP-IDF.
* Soporte para FreeRTOS.

---

# 🌡️ Sensors

## DS18B20 Temperature Sensor

Mide la temperatura del sistema.

---

## Soil Moisture Sensor

Permite conocer la humedad del sustrato.

---

## Water Level Sensor

Supervisa la disponibilidad de agua para el sistema.

---

# 🖥️ User Interface

## OLED Display SSD1306

Pantalla OLED 128x64 mediante comunicación I2C.

Muestra:

* Temperatura.
* Humedad.
* Nivel de agua.
* Estado del sistema.
* Estado MQTT.
* Alertas.

---

# ⚡ Actuators

## Climate Controller

Sistema comercial adaptado para control mediante ESP32.

Funciones:

* FAN (3 niveles).
* MIST / Humidificación (3 niveles).
* RGB Lighting.
* Timer.
* Alarm.

El control se realiza utilizando optoacopladores **PC817**, simulando la pulsación de botones sin modificar la electrónica original del dispositivo.

---

## Relay Module

Control de bomba de agua con aislamiento eléctrico.

---

## Indicators

LEDs:

* Estado normal.
* Estado de alarma.

---

## Buzzer

Sistema de alerta sonora.

---

# 🧠 Software Architecture

El firmware fue desarrollado utilizando:

## ESP-IDF

Framework oficial de Espressif para desarrollo profesional con ESP32.

---

# FreeRTOS Tasks

El sistema utiliza una arquitectura basada en tareas independientes.

## sensor_task

Responsable de:

* Lectura de temperatura.
* Lectura de humedad.
* Lectura de nivel de agua.

---

## control_task

Motor de decisiones del sistema.

Controla:

* Ventilador.
* Humidificador.
* Bomba.
* LEDs.
* Alarma.

---

## display_task

Actualización de información en pantalla OLED.

---

## mqtt_task

Comunicación con el dashboard mediante MQTT.

---

# 🌐 MQTT Communication

MQTT significa:

**Message Queuing Telemetry Transport**

Es un protocolo ligero diseñado para comunicación entre dispositivos IoT.

El ESP32 publica información del sistema y recibe comandos desde el dashboard.

---

## Publish Topics

```txt
kazthor/farm01/data

kazthor/farm01/status

kazthor/farm01/alerts
```

---

## Subscribe Topic

```txt
kazthor/farm01/cmd
```

---

# 📊 Web Dashboard

Dashboard desarrollado utilizando:

* HTML.
* CSS.
* JavaScript.
* MQTT over WebSockets.
* Chart.js.

Funciones:

* Visualización en tiempo real.
* Control manual de dispositivos.
* Estado del sistema.
* Gráficas.
* Comunicación bidireccional con ESP32.

---

# 🚦 System States

El sistema cuenta con tres estados principales.

---

# NORMAL

Condiciones:

```txt
Temperature < 35°C

Soil Moisture >= 40%
```

Acciones:

* Sistema estable.
* LED verde activo.
* Sin alarmas.

---

# ALERTA

Condición:

```txt
Soil Moisture < 40%
```

Acciones:

* Ajustes de climatización.
* Indicadores de advertencia.

---

# CRITICAL

Condiciones:

```txt
Temperature >= 35°C

OR

Soil Moisture < 20%
```

Acciones:

* FAN nivel 3.
* MIST nivel 3.
* Relay activado.
* LED rojo.
* Buzzer.

---

# 🔌 Wiring Summary

## OLED

```txt
SDA -> GPIO 8

SCL -> GPIO 9
```

---

## DS18B20

```txt
DATA -> GPIO 13

Pull-up 4.7kΩ
```

---

## Soil Sensor

```txt
GPIO 4
```

---

## LEDs

```txt
Green -> GPIO 10

Red -> GPIO 11
```

---

## Buzzer

```txt
GPIO 12
```

---

## Relay

```txt
GPIO 15
```

---

# 📁 Repository Structure

```txt
KAZTHOR-FARM-V3

│
├── firmware
│   └── ESP32-S3
│
├── dashboard
│
├── docs
│
├── media
│   ├── demo.gif
│   ├── dashboard.webp
│   └── prototype.webp
│
├── evidence
│
├── README.md
│
└── LICENSE
```

---

# 📸 Project Evidence

El repositorio incluye:

* Fotografías del hardware.
* Diagramas de conexión.
* Capturas del dashboard.
* Logs del sistema.
* Evidencia de pruebas.
* Videos de demostración.

---

# 🛣️ Roadmap

# ✅ KAZTHOR FARM V3

Estado actual:

* ESP32-S3 migration.
* ESP-IDF implementation.
* FreeRTOS architecture.
* MQTT communication.
* Web dashboard.
* OLED monitoring.
* Automatic climate control.
* FAN/MIST commercial device integration.

---

# 🚧 KAZTHOR FARM V4

Próximas mejoras:

* ESP32-S3 Camera.
* Captura de imágenes.
* Visión por computadora.
* Inteligencia artificial.
* Análisis del crecimiento de plantas.
* Historial de datos.
* Notificaciones automáticas.

---

# 🔮 Future Development

Planes a largo plazo:

* Diseño de PCB personalizada.
* Reemplazo de protoboard.
* Diseño de sistema propio de ventilación y humidificación.
* Componentes impresos en 3D.
* Control de dirección de aire mediante servomotor.
* Adaptación a diferentes métodos hidropónicos.
* Desarrollo de una versión comercial.

---

# 👨‍💻 Author

**Antonio Castor Silva**

KAZTHOR LABS

GitHub:

https://github.com/antoniokazthor12345

---

# 📄 License

MIT License

---

# ⭐ Support

If you find this project interesting, consider giving the repository a star and following future developments from KAZTHOR LABS.

Gracias por apoyar el desarrollo de proyectos relacionados con IoT, sistemas embebidos y automatización.
