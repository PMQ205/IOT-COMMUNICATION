#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
// cac chan arduino ra led va buzzer
const int button1 = 5;
const int button2 = 6;
const int button3 = 2;
const int button4 = 3;
const int buzzer = 4;


void setup() {
  pinMode(buzzer,OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  pinMode(button4,INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // Kiểm tra RTC
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
  }

  // Nếu RTC chưa cài giờ, cài giờ theo thời gian máy tính
  if (!rtc.isrunning()) {
    lcd.clear();
    lcd.print("RTC adjust...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    delay(1000);
  }

  lcd.clear();

}
static void display_time_now()
{
  DateTime now = rtc.now();
      lcd.setCursor(0, 0);
      lcd.print("Time: ");
      if (now.hour() < 10) lcd.print('0');// neu co 1 chu so thi in so 0 o dau
      lcd.print(now.hour());
      lcd.print(':');
      if (now.minute() < 10) lcd.print('0');
      lcd.print(now.minute());
      lcd.print(':');
      if (now.second() < 10) lcd.print('0');
      lcd.print(now.second());

      lcd.setCursor(0, 1);
      lcd.print("Date: ");
      if (now.day() < 10) lcd.print('0');
      lcd.print(now.day());
      lcd.print('/');
      if (now.month() < 10) lcd.print('0');
      lcd.print(now.month());
      lcd.print('/');
      lcd.print(now.year());
}
extern int alarm_time[]={0,0,0,0,0,0};
static void set_time(bool x,int index)
{
  // === Tăng giá trị (x = true) ===
    if (x)
    {
        alarm_time[index] += 1; // Tăng giá trị trước

        switch(index)
        {
            case 0: // Giờ (Giới hạn 24 giờ: 0-23)
            {
                if (alarm_time[index] > 23) { 
                    alarm_time[index] = 0; // Vượt quá 23, reset về 0
                }
                break;
            }
            case 1: // Phút (Giới hạn 0-59)
            case 2: // Giây (Giới hạn 0-59)
            {
                if (alarm_time[index] > 59) {
                    alarm_time[index] = 0; // Vượt quá 59, reset về 0
                }
                break;
            }
            case 3: // Ngày (Giới hạn phụ thuộc vào Tháng và Năm - Cần logic phức tạp hơn)
            {
                // Để đơn giản, giả sử giới hạn tối đa là 31
                if (alarm_time[index] > 31) {
                    alarm_time[index] = 1; // Vượt quá 31, reset về 1
                }
                break;
            }
            case 4: // Tháng (Giới hạn 1-12)
            {
                if (alarm_time[index] > 12) {
                    alarm_time[index] = 1; // Vượt quá 12, reset về 1
                }
                break;
            }
            case 5: // Năm (Giới hạn tuỳ chọn, ví dụ: 2000-2099)
            {
                if (alarm_time[index] > 2099) {
                    alarm_time[index] = 2000; // Vượt quá 2099, reset về 2000
                }
                break;
            }
        }
        
    }
    // === Giảm giá trị (x = false) ===
    else 
    {
        alarm_time[index] -= 1; // Giảm giá trị trước

        switch(index)
        {
            case 0: // Giờ (Giới hạn 0-23)
            {
                if (alarm_time[index] < 0) { 
                    alarm_time[index] = 23; // Dưới 0, vòng lại về 23
                }
                break;
            }
            case 1: // Phút (Giới hạn 0-59)
            case 2: // Giây (Giới hạn 0-59)
            {
                if (alarm_time[index] < 0) {
                    alarm_time[index] = 59; // Dưới 0, vòng lại về 59
                }
                break;
            }
            case 3: // Ngày (Giới hạn 1-31)
            {
                if (alarm_time[index] < 1) {
                    alarm_time[index] = 31; // Dưới 1, vòng lại về 31
                }
                break;
            }
            case 4: // Tháng (Giới hạn 1-12)
            {
                if (alarm_time[index] < 1) {
                    alarm_time[index] = 12; // Dưới 1, vòng lại về 12
                }
                break;
            }
            case 5: // Năm (Giới hạn 2000-2099)
            {
                if (alarm_time[index] < 2000) {
                    alarm_time[index] = 2099; // Dưới 2000, vòng lại về 2099
                }
                break;
            }
        }
    }
}
static void print_time(int index){
    int row = index>2?1:0;
    // int col =5 + index*2 + index;
    lcd.setCursor(6 + index * 2 + index ,row);
    if(alarm_time[index]<10) lcd.print('0'); 
    lcd.print(alarm_time[index]);
}
void loop() {
  // put your main code here, to run repeatedly:
  
  // static int button_state[]={0,0,0,0};
  static int mode_state=0;// mode xem gio 1, bao thuc 2, set time 3
  static bool old_mode=false;
  static int state_set_alarm=0;// 0 la gio , 1 la phut , 2 la giay , 3 la ngay, 4 la thang , 5 la nam
  static bool change=false; // kiem tra co thay doi hay khong , neu khong thay doi ma nhan set thi luu 
  
  static int index_alarm_time=0;


  if(mode_state==0)
  {
      lcd.noCursor();
      display_time_now();
      DateTime now = rtc.now();
      if(alarm_time[0] == now.hour() && alarm_time[1] == now.minute() && alarm_time[2] == now.second() && alarm_time[3] == now.day() && alarm_time[4] == now.month() && alarm_time[5] == now.year())
      {
        digitalWrite(buzzer, HIGH);
        delay(2000);
        digitalWrite(buzzer, LOW);
      }
  }
  else if(mode_state == 1 || mode_state == 2)
  {
      if(old_mode)
      {
        display_time_now();
        old_mode=false;
        DateTime now = rtc.now();
        // Gán giá trị vào mảng alarm_time
        alarm_time[0] = now.hour(); 
        alarm_time[1] = now.minute();
        alarm_time[2] = now.second();
        alarm_time[3] = now.day();
        alarm_time[4] = now.month();
        alarm_time[5] = now.year();
        lcd.cursor();
      }
      

      if(!digitalRead(button2))//tang
      {
        while(!digitalRead(button2)){};

        set_time(true,index_alarm_time);
        print_time(index_alarm_time);

        change=true;
      }
      if(!digitalRead(button3))//giam
      {
        while(!digitalRead(button3)){};
        set_time(false,index_alarm_time);
        print_time(index_alarm_time);

        change=true;
      }
      if(!digitalRead(button4))//set
      {
        while(!digitalRead(button4)){};
        if(change==true)// neu da thay doi gio ngay thi nhay den thoi gian tiep theo
        {
          index_alarm_time +=1;// tang vi tri thoi gian
          if(index_alarm_time>5) index_alarm_time=0;
        }
        if(change==false)// neu ko co thay doi gi thi luu thiet lap
        {
          lcd.noCursor();
          mode_state=0;
          lcd.clear();
          lcd.home();
          lcd.print("Complete");
          lcd.setCursor(0,1);
          lcd.print("alarm set up");
          delay(500);
          if(mode_state==3)
          {
            DateTime set(alarm_time[5], alarm_time[4], alarm_time[3], 
               alarm_time[0], alarm_time[1], alarm_time[2]);
            rtc.adjust(set);
          }

        }
      }
      
  }
  // nhan button1 doi mode 
  if(!digitalRead(button1))
  {
    while(!button1){};// chờ nhả nút
    mode_state++;
    old_mode=true;

    if(mode_state>2) mode_state=0;
    lcd.clear();
    lcd.home();
    delay(50);
    if(mode_state==0) 
    {
      lcd.print("Che do xem gio");
      delay(500);
      lcd.clear();
    }
    else if(mode_state==1)
    {
      lcd.print("Che do ");
      lcd.setCursor(0,1);
      lcd.print("cai bao thuc");
      delay(500);
      lcd.clear();
    }
    else 
    {
      lcd.print("Che do");
      lcd.setCursor(0,1);
      lcd.print("cai thoi gian");
      delay(500);
      lcd.clear();
    }
  }
}
