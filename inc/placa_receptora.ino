#include "funciones.h"

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    inicializarFiltros();

    ultimoSonidoGlobal = millis();

    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, MAC_TRANSMISOR, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    enviarEstado(0);

    // ======================================================
    // CREACIÓN DE TAREAS FREERTOS
    // ======================================================
    xTaskCreatePinnedToCore(
        TaskLecturaMicro,
        "LecturaMicro",
        4096,
        NULL,
        2,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        TaskDeteccionApnea,
        "DeteccionApnea",
        4096,
        NULL,
        1,
        NULL,
        1
    );

    Serial.println("Sistema con FreeRTOS INICIADO.");
}

void loop() {
    // No se usa. FreeRTOS maneja todo.
}

