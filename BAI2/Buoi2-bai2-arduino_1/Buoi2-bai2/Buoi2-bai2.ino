const int btn = 2;// cấu hình nút nhấn cho chân 2
int current_baudrate = 9600;
bool mode = false;// trạng
unsigned long thoiDiemBatDauNhan = 0;

void setup() {
  Serial.begin(current_baudrate);
  pinMode(btn, INPUT); // nút nhấn kéo xuống
}

void loop() {
  static int lastButtonState = LOW;
  int buttonState = digitalRead(btn);

  // PHÁT HIỆN NHẤN
  if (buttonState == HIGH && lastButtonState == LOW) {
    thoiDiemBatDauNhan = millis();
  }

  // PHÁT HIỆN NHẢ NÚT
  if (buttonState == LOW && lastButtonState == HIGH) {
    unsigned long thoiGianNhan = millis() - thoiDiemBatDauNhan;

    if (thoiGianNhan >= 1000) { // nhấn >1s -> bật/ tắt chế độ cấu hình
      mode = !mode;
      Serial.print(mode ? "1\n" : "0\n");
    }
    else if (thoiGianNhan > 50 && mode) { // nhấn ngắn -> đổi baud
      Serial.println("changemode");      // gửi lệnh đổi baud
      // Chờ ACK từ Arduino 2
      unsigned long startWait = millis();
      while (millis() - startWait < 1000) { // tối đa 1mls chờ
        if (Serial.available() > 0) {
          String confirm = Serial.readStringUntil('\n');
          confirm.trim();
          if (confirm == "OK") {
            // Đổi baud sau khi Arduino 2 đã xác nhận
            if (current_baudrate == 9600) current_baudrate = 115200;
            else current_baudrate = 9600;
            Serial.end();
            Serial.begin(current_baudrate);
            break;
          }
        }
      }
    }
  }

  lastButtonState = buttonState;
  
  // Nhận dữ liệu từ Arduino 2 (ví dụ: báo Baud hiện tại)
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();
    if (!mode && receivedData.length() > 0 && isDigit(receivedData[0])) {
      current_baudrate = receivedData.toInt();
      Serial.print("Da nhan Baud Rate tu Arduino 2: ");
      Serial.println(current_baudrate);
    }
  }
}
