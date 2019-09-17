#include <ESP8266WiFi.h>

// Change the SSID according to your network
char ssid[] = "LEARN_WIFI";
// Change the Password according to your network
char pass[] = "learnmore";

char server[] = "rss.detik.com";
int status = WL_IDLE_STATUS; //fungsi library esp

WiFiClient client;

void pageRequest() {
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    client.println("GET /rss.php HTTP/1.1");
    client.println("Host: rss.detik.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  pageRequest();
}

void loop() {

if (client.available()) { //ngecek tersedia atau ngg
    char c = client.read(); //clien ngebaca server perkarakter
    Serial.print(c);
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    delay(1000);
    client.stop();
  }  
}
