#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor



// Remova o comentário de uma das linhas abaixo para qualquer tipo de sensor DHT que você está usando!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

/* Coloque seu SSID e senha*/
const char* ssid = "Medeiros_2G";  // Entre SSID aqui
const char* password = "rnppgaxa4n";  // Insira a senha aqui

WebServer server(80);

// Sensor DHT
uint8_t DHTPin = 4; 
               
// Inicializa o sensor DHT.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {

  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();              

  Serial.println("Conectando à ");
  Serial.println(ssid);

  // conecte-se à sua rede wi-fi local
  WiFi.begin(ssid, password);

  // verifique se o wi-fi está conectado à rede wi-fi
  while (WiFi.status() != WL_CONNECTED) {
     delay(1000);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado..!");
  Serial.print("IP obtido: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
//sensor moisture
  
//fim
}


void handle_OnConnect() {

 Temperature = dht.readTemperature(); // Obtém os valores da temperatura
  Humidity = dht.readHumidity(); // Obtém os valores da umidade
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Não encontrado");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Sensor Temperatura ESP32</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Sensor Temperatura ESP32</h1>\n";
  
  ptr +="<p>Temperatura do ambiente: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Umidade do ambinte: ";
  ptr +=(int)Humiditystat; 
  ptr +="%</p>";


  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}