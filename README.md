# ProyectoDetectordeApnea

## Sistema Embebido para Detección de Apnea Neonatal

Este proyecto implementa un sistema embebido basado en **ESP32 + FreeRTOS**, diseñado para detectar episodios de **apnea neonatal** mediante la medición simultánea de:

- Señales acústicas de respiración usando un **micrófono INMP441**
- Micro-movimientos torácicos usando un **sensor piezoeléctrico**

Si ambos sensores detectan ausencia de respiración y movimiento durante **15 segundos**, el sistema activa un **buzzer de alarma**.  
El objetivo es crear un dispositivo **económico, confiable y accesible** para incubadoras y monitoreo neonatal.

# Arquitectura del Sistema 


<h2 align="center">Arquitectura del Sistema</h2>

<p align="center">
  <img src="https://raw.githubusercontent.com/marc720/ProyectoDetectordeApnea/main/img/diagrama.jpg" width="700">
</p>

##  Tecnologías Utilizadas

### Hardware
- ESP32 WROOM
- Micrófono INMP441 (I2S o ADC temporal)
- Sensor piezoeléctrico
- Buzzer piezoeléctrico
- Fuente 5V/3.3V
- Cuna o incubadora para pruebas

### Software
- Lenguaje C/C++ (Arduino framework)
- FreeRTOS (tareas, FSM, ticks)
- Máquinas de estados (FSM para micrófono, piezo y buzzer)
- Git y GitHub para control de versiones
- Visual Studio Code + PlatformIO / Arduino IDE


## Integrantes y Roles

| Integrante | Rol |
|-----------|------|
| *Marcia Santa Maria* | Integración FreeRTOS, lógica de apnea, documentación |


## Estado Actual del Proyecto

### ✔ Completado
- Carcasa física y montaje final
- FSM del buzzer (alarma)
- Integración de sensores al ESP32
- Lógica de apnea de 15 segundos basada en FreeRTOS
- Código modular (funciones.cpp / funciones.h / .ino)
- Arquitectura general definida
- Carpeta `/img` agregada al repositorio

### En Progreso
- Sustitución de lectura ADC por lectura real I2S para INMP441
- Pruebas clínicas simuladas
- Integración de filtros de señal (RMS para audio)

### Pendiente
- Conectividad (posible ESP-NOW / WiFi)
- Documentación de instalación y pruebas
- Implementación de FSM del micrófono
- Implementación de FSM del sensor piezoeléctrico