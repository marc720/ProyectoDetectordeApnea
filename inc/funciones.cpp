#include "funciones.h"

// ===== Variables del sistema =====
uint8_t MAC_TRANSMISOR[6] = {0x20, 0x43, 0xA8, 0x63, 0xD6, 0x8C};

struct_message datosAEnviar;
struct_message datosRecibidos;

// Pines
const int MIC_PIN = 34;
const int LED_PIN = 13;

// Parámetros acústicos
const int NUM_MUESTRAS = 200;
const int T_MUESTRA_US = 200;
const float ALPHA_HPF = 0.9696f;
const float ALPHA_LPF = 0.6909f;
const int UMBRAL_SONIDO = 2000;
const unsigned long TIEMPO_SILENCIO = 15000;

// Variables de filtro
static float x_prev = 0;
static float hpf_prev = 0;
static float lpf_prev = 0;

// Variables globales compartidas entre tareas
volatile int amplitudGlobal = 0;
volatile unsigned long ultimoSonidoGlobal = 0;
volatile bool estadoApneaGlobal = false;

// ----------------------------------------------------------
// INICIALIZA LOS FILTROS
// ----------------------------------------------------------
void inicializarFiltros() {
    Serial.println("INICIALIZANDO FILTROS...");

    x_prev = analogRead(MIC_PIN);
    hpf_prev = 0;
    lpf_prev = 0;

    for (int i = 0; i < 20; i++) {
        leerAmplitud();
    }

    Serial.println("Filtros ESTABILIZADOS.");
}

// ----------------------------------------------------------
// LECTURA DE AMPLITUD FILTRADA
// ----------------------------------------------------------
int leerAmplitud() {
    int minVal = 4095;
    int maxVal = 0;

    for (int i = 0; i < NUM_MUESTRAS; i++) {
        float muestra = analogRead(MIC_PIN);

        float y_hpf = ALPHA_HPF * (hpf_prev + muestra - x_prev);
        x_prev = muestra;
        hpf_prev = y_hpf;

        float y_lpf = (1 - ALPHA_LPF) * y_hpf + ALPHA_LPF * lpf_prev;
        lpf_prev = y_lpf;

        if (y_lpf < minVal) minVal = y_lpf;
        if (y_lpf > maxVal) maxVal = y_lpf;

        delayMicroseconds(T_MUESTRA_US);
    }

    return maxVal - minVal;
}

// ----------------------------------------------------------
// ENVÍO ESP-NOW
// ----------------------------------------------------------
void enviarEstado(int estado) {
    datosAEnviar.estadoAlarma = estado;
    datosAEnviar.volumen = 0;

    esp_now_send(MAC_TRANSMISOR, (uint8_t*)&datosAEnviar, sizeof(datosAEnviar));
}

// ----------------------------------------------------------
// CALLBACK DE RECEPCIÓN ESP-NOW
// ----------------------------------------------------------
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if(len < sizeof(struct_message)) return;

    memcpy(&datosRecibidos, data, sizeof(datosRecibidos));

    if (datosRecibidos.estadoAlarma == 99) {
        Serial.println("Comando RESET (99) recibido. Reiniciando...");
        delay(300);
        ESP.restart();
    }
}

// ==========================================================
// ============= TAREA 1: LECTURA DEL MICRÓFONO =============
// ==========================================================
void TaskLecturaMicro(void *pvParameters) {
    while (true) {
        int amp = leerAmplitud();
        amplitudGlobal = amp;

        if (amp > UMBRAL_SONIDO) {
            ultimoSonidoGlobal = millis();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ==========================================================
// ============= TAREA 2: DETECCIÓN DE APNEA ================
// ==========================================================
void TaskDeteccionApnea(void *pvParameters) {
    while (true) {
        unsigned long ahora = millis();
        unsigned long silencio = ahora - ultimoSonidoGlobal;

        if (silencio >= TIEMPO_SILENCIO) {
            if (!estadoApneaGlobal) {
                estadoApneaGlobal = true;
                digitalWrite(LED_PIN, HIGH);
                enviarEstado(1);
            }
        } else {
            if (estadoApneaGlobal) {
                estadoApneaGlobal = false;
                digitalWrite(LED_PIN, LOW);
                enviarEstado(0);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
