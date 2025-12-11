#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>

// ============ MAC DEL TRANSMISOR ============
extern uint8_t MAC_TRANSMISOR[6];

// ============ ESTRUCTURA DE DATOS ============
typedef struct struct_message {
  int estadoAlarma; 
  int volumen;
} struct_message;

// Globales
extern struct_message datosAEnviar;
extern struct_message datosRecibidos;

// Pines
extern const int MIC_PIN;
extern const int LED_PIN;

// Parámetros acústicos
extern const int NUM_MUESTRAS;
extern const int T_MUESTRA_US;
extern const float ALPHA_HPF;
extern const float ALPHA_LPF;
extern const int UMBRAL_SONIDO;
extern const unsigned long TIEMPO_SILENCIO;

// Variables globales compartidas entre tareas
extern volatile int amplitudGlobal;
extern volatile unsigned long ultimoSonidoGlobal;
extern volatile bool estadoApneaGlobal;

// Prototipos
void inicializarFiltros();
int leerAmplitud();
void enviarEstado(int estado);
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len);

// Prototipos de tareas FreeRTOS
void TaskLecturaMicro(void *pvParameters);
void TaskDeteccionApnea(void *pvParameters);

#endif
