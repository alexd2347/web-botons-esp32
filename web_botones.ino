#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "WiFi UVAQ";
const char* password = "";

AsyncWebServer server(80);

// Pines de los botones
const int pinBoton1 = 13;
const int pinBoton2 = 12;
const int pinBoton3 = 14;
const int pinBoton4 = 27;

// Estados de los botones
bool botonPresionado[4] = {false, false, false, false};

void setup() {
  Serial.begin(115200);

  // Conexión a la red WiFi
  Serial.println("Conectando a la red WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado");
  // Imprimir IP local en el Monitor Serie
  Serial.println(WiFi.localIP());

  // Iniciar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Inicialización del servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/estado", HTTP_GET, [](AsyncWebServerRequest* request) {
    String estadoBotones = "";
    for (int i = 0; i < 4; i++) {
      estadoBotones += String(botonPresionado[i]);
    }
    request->send(200, "text/plain", estadoBotones);
  });

  server.begin();

  // Configurar pines de los botones como entradas
  pinMode(pinBoton1, INPUT_PULLUP);
  pinMode(pinBoton2, INPUT_PULLUP);
  pinMode(pinBoton3, INPUT_PULLUP);
  pinMode(pinBoton4, INPUT_PULLUP);
}

void loop() {
  // Leer el estado de los botones y actualizar el arreglo
  botonPresionado[0] = !digitalRead(pinBoton1);
  botonPresionado[1] = !digitalRead(pinBoton2);
  botonPresionado[2] = !digitalRead(pinBoton3);
  botonPresionado[3] = !digitalRead(pinBoton4);
  delay(10);  // Pequeña pausa para evitar rebotes
}
