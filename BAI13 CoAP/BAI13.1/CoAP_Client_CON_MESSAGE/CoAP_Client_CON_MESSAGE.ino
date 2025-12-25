#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <WiFiUdp.h>
#include <coap-simple.h>

const char* WIFI_SSID = "REDMI Turbo 4 Pro";
const char* WIFI_PASS = "quandznhattg";

/* IP MÁY CHẠY SERVER */
IPAddress coap_server_ip(10.153.38.194);
const int coap_port = 5683;

const char* coap_resource = "sensor/temp";
const unsigned long SEND_INTERVAL = 5000;

WiFiUDP udp;
Coap coap(udp);

unsigned long lastSend = 0;

void setup_wifi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  coap.start();
  Serial.println("CoAP client started");
}

void loop() {
  if (millis() - lastSend > SEND_INTERVAL) {
    lastSend = millis();

    String payload = "temp=30.5";
    Serial.println("Sending CoAP NON message");

    coap.send(
      coap_server_ip,
      coap_port,
      coap_resource,
      COAP_NON,     // 🔹 NON
      COAP_POST,
      (uint8_t*)payload.c_str(),
      payload.length()
    );
  }
}
