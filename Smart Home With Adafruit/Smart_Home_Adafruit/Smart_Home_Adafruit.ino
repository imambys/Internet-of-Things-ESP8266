#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

#define Relay1  5
#define Relay2  12
#define DHTPIN  4
#define Maghnetpin  4
#define DHTTYPE DHT22

#define WLAN_SSID       "Your_SSID"            // Your SSID
#define WLAN_PASS       "Your_Password"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"     //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Your_Username"       // Username
#define AIO_KEY         "Your_AIOKEY"         // Auth Key

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client; //WIFI CLIENT

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish Suhu = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Suhu");          // feed for Temperature
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");  // feed for humidity
Adafruit_MQTT_Publish Door = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Door");          // feed for maghnet sensor

Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1");  // Feed for Relay 1
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");  // Feed for Relay 2

float h, t;

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Maghnetpin, INPUT_PULLUP); // Magnet
  pinMode(DHTPIN, INPUT_PULLUP);     // DHT
  pinMode(Relay1, OUTPUT);          // Lampu 1
  pinMode(Relay2, OUTPUT);          // Lampu 2

  dht.begin();
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
}

void loop() {
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Relay1, Light1_State);
      
    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Relay2, Light2_State);
    }
  }

  // Read Temperature and Humidity Sensor
  t = dht.readTemperature();      // read a Temperature in Celcius degree (°C)
  h = dht.readHumidity();         // read a humidity in percent (%)
 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
  
  Serial.print(F("\nSending Suhu val "));
  Serial.print(t);
  Serial.print("...");
  if (! Suhu.publish(t++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  Serial.print(F("\nSending Humidity val "));
  Serial.print(h);
  Serial.print("...");
  if (! Humidity.publish(h++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Read Maghnet Sensor
  boolean door = digitalRead(13);
  Serial.print(F("\nSending Door val "));
  Serial.print(door);
  Serial.print("...");
  if (! Door.publish(door++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
