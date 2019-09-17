#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define lampu1  D4  //relay1 
#define lampu2  D5  //relay2
#define magnetpin  D7  //Magnetic switch Sensor
#define dhtpin     D8  //Temperature and Humidity Sensor

#define DHTTYPE DHT22   // DHT22  (AM2302)
DHT dht(dhtpin, DHTTYPE);
float h, t; 
String data, data2;

ESP8266WebServer server(80);

char ssid[] = "ESP_Wifi";
char password[] = "11223344";
String webPage = "";

IPAddress IP(192, 168, 3, 10);
IPAddress mask = (255, 255, 255, 0);

void setup(void) {
  webPage += "<h1>Smart Home Lokal Only</h1><p>Lampu 1 <a href=\"nyala\"><button>ON</button></a>&nbsp;<a href=\"mati\"><button>OFF</button></a></p><p>Lampu 2 <a href=\"l2nyala\"><button>ON</button></a>&nbsp;<a href=\"l2mati\"><button>OFF</button></a></p>";//<p>Temperature : t &nbsp;Humidity : h</p>";

  pinMode(lampu1, OUTPUT);
  pinMode(lampu2, OUTPUT);
  pinMode(magnetpin, INPUT_PULLUP);      // For Magnetic Reed Switch
  pinMode(dhtpin, INPUT_PULLUP);       // For DHT22
  digitalWrite(lampu1, LOW);
  digitalWrite(lampu2, LOW);
  Serial.begin(9600);
  dht.begin();
  delay(10);

  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  WiFi.softAPConfig(IP, IP, mask);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", webPage + data + data2);
  });

  //Nyalakan Lampu 1
  server.on("/nyala", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lampu1, HIGH);
    delay(50);
  });

  //Matikan Lampu 1
  server.on("/mati", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lampu1, LOW);
    delay(50);
  });

  //Nyalakan Lampu 2
  server.on("/l2nyala", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lampu2, HIGH);
    delay(50);
  });

  //Matikan Lampu 2
  server.on("/l2mati", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lampu2, LOW);
    delay(300);
  });
  
  server.begin();
  Serial.println("HTTP server started");

}

void loop(void) {
  // Humidity and Temperature
  h = dht.readHumidity();
  t = dht.readTemperature();
  data = "";
  data += "<p>";
  data += "Humidity : ";
  data += h;
  data += "%  Temperature : ";
  data += t;
  data += "C";
  data += "</p>";
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%");
  Serial.print(" ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  //Sensor Magnet
  boolean btnState = digitalRead(13);
  if (btnState != LOW) {
    data2 = "";
    data2 += "<p>";
    data2 += "Sensor Status : ";
    data2 += "The DOOR IS OPEN!";
    data2 += "</p>";
  } else {
    data2 = "";
    data2 += "<p>";
    data2 += "Sensor Status : ";
    data2 += "The House is Safe!";
    data2 += "</p>";
  }
  
  server.handleClient();
}
