#include <WiFi.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <WebServer.h>
#include <ESP32Servo.h> //Biblioteca para el servo motor

const int servoPin = 32;
const int sensorPin = 33;
Servo servo;

const char *ssid = "PEINE-3";
const char *password = "etecPeine3";
const int port = 5000;
const int OK_CODE = 200;
const int NOT_FOUND_CODE = 404;

WebServer server(port);

unsigned long ultimoTiempo = 0;
const long tiempoEspera = 3000;

void setup() {
 Serial.begin(115200);
 servo.attach(servoPin);
 servo.write(0);
 Serial.println();
 WiFi.begin(ssid, password);


 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }


 Serial.println("");
 Serial.println("Wifi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 Serial.println("Servidor corriendo en puerto...");
 Serial.println(port);
 server.on("/api/clima",HTTP_POST, recibirClima); //Si no pongo HTTP_POST, va a recibir un GET
 server.onNotFound(noEncontrada);
 //puedo configurar otras rutas
 server.begin();
}

void recibirClima()
{
 Serial.print("Se recibió una conexión!");
 String body= server.arg("plain");
 Serial.print("Contenido:");
 Serial.println(body);
 JsonDocument doc;
 deserializeJson(doc,body);
 bool llueve = doc["llueve"];
 Serial.print("Llueve?:");
 Serial.println(llueve);
 if (llueve) {
   servo.write(90);
 } else {
   servo.write(0);
 }
 server.send(OK_CODE, "text/html","<h1>OK</h1>"); //MIME TYPES
}


void noEncontrada()
{
 Serial.print("Se recibió una conexión a una ruta desconocida");
 server.send(NOT_FOUND_CODE, "text/html","<h1>Ruta desconoocida, use api/servo</h1>"); //MIME TYPES
}


void loop() {
 if (WiFi.status() == WL_CONNECTED) {
   server.handleClient();
} else {
   Serial.println("Se desconectó del wifi");
}
}
