#include <DHT.h>

#define DHTPIN 2       // Chân DATA của DHT11
#define DHTTYPE DHT11  // Loại cảm biến

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // UART -> Bluetooth -> PC
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // °C

  // Nếu đọc lỗi
  if (isnan(h) || isnan(t)) {
    Serial.println("ERR");
  } else {
    // Gửi dạng CSV: temp,hum\n  VD: 27.5,63.0
    Serial.print(t, 1);   // 1 chữ số sau dấu phẩy
    Serial.print(",");
    Serial.println(h, 1);
  }

  delay(2000); // 2 giây gửi 1 lần
}
