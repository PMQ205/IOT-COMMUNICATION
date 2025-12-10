const int led_phanHoi = 5;
const int led_9600 = 6;
const int led_115200 = 7;

int currentBaudRate = 9600;
bool che_do_cau_hinh = false;
unsigned long thoiDiemNhay = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led_phanHoi, OUTPUT);
  pinMode(led_9600, OUTPUT);
  pinMode(led_115200, OUTPUT);

  digitalWrite(led_9600, HIGH); // mặc định 9600
  digitalWrite(led_115200, LOW);
  digitalWrite(led_phanHoi, LOW);
}

void loop() {
  // 1. Nhận lệnh từ Arduino 1
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();

    if (receivedData == "1") {
      che_do_cau_hinh = true;
      thoiDiemNhay = millis();
    }
    else if (receivedData == "0") {
      che_do_cau_hinh = false;
      Serial.println(currentBaudRate); // gửi Baud hiện tại về Master
    }
    else if (receivedData == "changemode") {
      // Đổi baud
      if (currentBaudRate == 9600) {
        currentBaudRate = 115200;
        digitalWrite(led_115200, HIGH);
        digitalWrite(led_9600, LOW);
      } else {
        currentBaudRate = 9600;
        digitalWrite(led_115200, LOW);
        digitalWrite(led_9600, HIGH);
      }
      Serial.println("OK"); // thông báo Master trước khi đổi baud
      delay(100);             // đảm bảo Serial gửi kịp
      Serial.end();
      Serial.begin(currentBaudRate);
    }
  }

  // 2. Nháy LED phản hồi non-blocking
  if (che_do_cau_hinh) {
    if (millis() - thoiDiemNhay >= 50) {
      digitalWrite(led_phanHoi, !digitalRead(led_phanHoi));
      thoiDiemNhay = millis();
    }
  } else {
    digitalWrite(led_phanHoi, LOW);
  }
}
