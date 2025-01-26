// Подключение бибилиотек
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
 
// Указание пинов для измерения напряжения на резисторах

#define c1 A1
#define c2 A2
#define c3 A3
#define c4 A4
#define c5 A5
#define c6 A6 
#define c7 A7
#define c8 A8
#define c9 A9
#define c10 A10
#define c11 A11

// Потенциометры регулировки напряжения
#define VoltageRegulator1 A15
#define VoltageRegulator2 A14
// Вывод напряжения
#define OutputVoltage1 8
#define OutputVoltage2 9

#define relay1 42
#define relay2 44

// Кнопка
#define buttonPin 48
#define buttonRelay 46
// Адрес I2C LCD дисплея, может быть 0x27 или 0x3F
#define I2C_ADDR 0x27
// Количество столбцов и строк на LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2
 
// Инициализация LCD дисплея через I2C
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);
 
// Установка опорного напряжения для АЦП (В данном случает DEFAULT = 9В)
int ReferenceVoltage = 5; 
 
 
// Сопротивления известных резисторов
const int R1 = 500;
const int R2 = 550;
const int R3 = 1000;
const int R4 = 550;
const int R5 = 220;
const int R6 = 350;
const int R7 = 550;
const int R8 = 220;
 
// Переменные вывода на экран
int buttonState = 0;    // Текущее состояние кнопки
int lastButtonState = 0; // Предыдущее состояние кнопки
int currentScreen = 0;    // Текущий экран
int buttonRelayState = 0;
 
void setup() {
    Serial.begin(9600);
    // Установка режима работы пинов 
    pinMode(c1, INPUT);
    pinMode(c2, INPUT);
    pinMode(c3, INPUT);
    pinMode(c4, INPUT);
    pinMode(c5, INPUT);
    pinMode(c6, INPUT);
    pinMode(c7, INPUT);
    pinMode(c8, INPUT);
    pinMode(c9, INPUT);
    pinMode(c10, INPUT);
    pinMode(c11, INPUT);
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(buttonRelay, INPUT_PULLUP);
    pinMode(VoltageRegulator1, INPUT);
    pinMode(VoltageRegulator2, INPUT);
    pinMode(OutputVoltage1, OUTPUT);
    pinMode(OutputVoltage2, OUTPUT);
    analogReference(DEFAULT);
    // Инициализация LCD и кнопки
    Wire.begin(); // Инициализация I2C
    lcd.init();    // Инициализация LCD
    lcd.backlight(); // Включаем подсветку LCD
    pinMode(buttonPin, INPUT_PULLUP);
}
 
void loop() {
    // Настройка напряжения на источниках E1 и E2
    int reqV1 = analogRead(VoltageRegulator1);
    int reqV2 = analogRead(VoltageRegulator2);
    analogWrite(OutputVoltage1, reqV1/4); 
    analogWrite(OutputVoltage2, reqV2/4);
    // Рассчет напряжений на точках подключения
    int V1 = map((analogRead(c10) - analogRead(c1)), 0, 1023, 0, ReferenceVoltage*1000);
    int Vpot3 = map((analogRead(c11) - analogRead(c10)), 0, 1023, 0, ReferenceVoltage*1000);
    int V2 = map((analogRead(c4) - analogRead(c11)), 0, 1023, 0, ReferenceVoltage*1000);
    int V3 = map((analogRead(c9) - analogRead(c1)), 0, 1023, 0, ReferenceVoltage*1000);
    int Vpot2 = map((analogRead(c8) - analogRead(c9)), 0, 1023, 0, ReferenceVoltage*1000);
    int V4 = map((analogRead(c4) - analogRead(c8)), 0, 1023, 0, ReferenceVoltage*1000);
    int Vpot1 = map((analogRead(c7) - analogRead(c8)), 0, 1023, 0, ReferenceVoltage*1000);
    int V5 = map((analogRead(c6) - analogRead(c7)), 0, 1023, 0, ReferenceVoltage*1000);
    int V6 = map((analogRead(c3) - analogRead(c4)), 0, 1023, 0, ReferenceVoltage*1000);
    int V7 = map((analogRead(c5) - analogRead(c4)), 0, 1023, 0, ReferenceVoltage*1000);
    int V8 = map((analogRead(c2) - analogRead(c1)), 0, 1023, 0, ReferenceVoltage*1000);
    int E2 = map((analogRead(c5) - analogRead(c6)), 0, 1023, 0, ReferenceVoltage*1000);
    int E1 = map((analogRead(c3) - analogRead(c2)), 0, 1023, 0, ReferenceVoltage*1000);
    // Рассчет токов по напряжению
    int I1 = V1/R1;
    int Ipot3 = I1;
    int I2 = V2/R2;
    int I3 = V3/R3;
    int Ipot2 = I3;
    int I4 = V4/R4;
    int I5 = V5/R5;
    int Ipot1 = I5;
    int I6 = V6/R6;
    int I7 = V7/R7;
    int I8 = V8/R8;
    
    Serial.println(analogRead(VoltageRegulator1));
    String screen1[] = {
        " E1 = " + String(E1),
        " E2 = " + String(E2)
    };
    
    String screen2[] = {
        "I1=" + String(I1) + " I2=" + String(I2),
        "I3=" + String(I3) + " I4=" + String(I4)
    };
    
    String screen3[] = {
        "I5=" + String(I5) + " I6=" + String(I6),
        "I7=" + String(I7) + " I8=" + String(I8)
    };
    
     String screen4[] = {
        "Ip1=" + String(Ipot1) + " Ip2=" + String(Ipot2),
        "Ip3=" + String(Ipot3)
    };
     
    String screen5[] = {
        "V1=" + String(V1) + " V2=" + String(V2),
        "V3=" + String(V3) + " V4=" + String(V4)
    };
    
    String screen6[] = {
        "V5=" + String(V5) + " V6=" + String(V6),
        "V7=" + String(V7) + " V8=" + String(V8)
    };

    String screen7[] = {
        "Vp1=" + String(Vpot1) + " Vp2=" + String(Vpot2),
        "Vp3=" + String(Vpot3)
    };
    // Вывод данных на экран
    // Считываем состояние кнопки
    buttonState = digitalRead(buttonPin);
    buttonRelayState = digitalRead(buttonRelay);
    if (buttonRelayState == LOW){
      digitalWrite(relay1, HIGH);  
      digitalWrite(relay2, HIGH);
    }
    else{
      digitalWrite(relay1, LOW);  
      digitalWrite(relay2, LOW); 
    }
    // Если кнопка нажата (переход от HIGH к LOW)
    if (buttonState == LOW && lastButtonState == HIGH) {
        currentScreen++;           // Переключаем на следующий раздел
        if (currentScreen > 6) {
            currentScreen = 0; // Переходим к первому разделу, если достигли последнего
        }
    }
    displayScreen(currentScreen, screen1, screen2, screen3, screen4, screen5, screen6, screen7); // Обновляем экран
    lastButtonState = buttonState; // Сохраняем текущее состояние кнопки
}
 
void displayScreen(int screenNumber, String screen1f[], String screen2f[], String screen3f[], String screen4f[], String screen5f[], String screen6f[], String screen7f[]) {
  static uint32_t tmr;
  if (millis() - tmr >= 500){
    tmr = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    switch (screenNumber) {
        case 0:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen1f[i]);
            }
            break;
 
        case 1:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen2f[i]);
            }
            break;
 
        case 2:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen3f[i]);
            }
            break;
        case 3:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen4f[i]);
            }
            break;
 
        case 4:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen5f[i]);
            }
            break;
 
        case 5:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen6f[i]);
            }
            break;
        case 6:
            for (int i = 0; i < 2; i++) {
                lcd.setCursor(0, i);
                lcd.print(screen7f[i]);
            }
            break;
    }
  }
}
