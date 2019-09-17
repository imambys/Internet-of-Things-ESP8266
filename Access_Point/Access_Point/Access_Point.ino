#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "test";
const char *password = "12345678"; //harus 8 karakter

ESP8266WebServer server(80); //Web server is on port 80, you can use other ports also, default HTTP port is 80

void webPage() {
  server.send(200,"text/html","<h1>Selamat Datang</h1><p>Hello</p>"); //header (200)
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", webPage);
  server.begin();                         //start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); //mengnangani client minta halaman http (web)
}

