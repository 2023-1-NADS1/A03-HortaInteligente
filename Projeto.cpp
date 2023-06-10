#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

int sensor_pin = 36;
int valueSoil ;

// Remova o comentário de uma das linhas abaixo para qualquer tipo de sensor DHT que você está usando!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "YourNetworkName";  // Enter SSID here
const char* password = "YourPassword";  //Enter Password here

WebServer server(80);
// Sensor DHT
uint8_t DHTPin = 18;
 
// Inicializa o sensor DHT.
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;
int Soil;
int Light

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LIGHT_SENSOR_PIN 32 // ESP32 pin GIOP36 (ADC0)


int rele = 16;




byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,};


void setup() {
Serial.begin(115200);
delay(100);
pinMode (5, OUTPUT); //l293d
pinMode(DHTPin, INPUT);
dht.begin();

pinMode(rele, OUTPUT); // initialize rele

 lcd.init();
 lcd.backlight();
 lcd.clear(); //Limpa o Display LCD.
//Cria o Caractere Customizado com o Símbolo do °.
lcd.createChar(0, grau);

Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}

}

void loop() {

  server.handleClient();
  

  float h = dht.readHumidity(); //É feito a Leitura do Valor da Umidade.
  float t = dht.readTemperature(); //É feito a Leitura do Valor da temperatura.
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7,0);
  lcd.print(t,1);
  lcd.setCursor(12,0);
  
  //Exibe o Símbolo do ° no Display LCD.
  lcd.write((byte)0);
  
  //Exibe o Símbolo da % no Display LCD.
  //lcd.print((char)223);
  
  lcd.setCursor(0,1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7,1);
  lcd.print(h,1);
  lcd.setCursor(12,1);
  lcd.print("%");
  
  //Intervalo Mínimo Recomendado para Fazer a Leitura dos Dados do Sensor.
  delay(2000);

  // reads the input on analog pin (value between 0 and 4095)
  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  Serial.println(analogValue);   // the raw analog reading


 
  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 50) {
    digitalWrite(rele, HIGH);
  } else {
    digitalWrite(rele, LOW );

  }


  valueSoil = analogRead(sensor_pin);
  Serial.print("Moisture : ");
  Serial.println(valueSoil);

   //the DC motor :
    if (valueSoil > 3000 ) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  } 
}


void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  Light  = map(LIGHT_SENSOR_PIN, 1023, 0, 0, 100);
  Soil = ( 100 - ( (sensor_pin/4095.00) * 100 ) );
  
  server.send(200, "text/html", SendHTML(Temperature,Humidity,Light,Soil)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat,int Lightstat, int Soilstat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Horta Inteligente</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Horta Inteligente</h1>\n";
  
  ptr +="<p>Temperatura do ar: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humimidade do ar: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  ptr +="<p>Quantidade de luz: ";
  ptr +=(int)Lightstat;
  ptr +="%</p>";
  ptr +="<p>Humidade do Solo: ";
  ptr +=(int)Soilstat;
  ptr +="%</p>";
    
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

