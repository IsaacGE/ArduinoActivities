#include <LCD_I2C.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LCD_I2C lcd(0x27);

String datos = "";
byte humi_act = 0;
byte humi_ant = 0;
byte temp_act = 0;
byte temp_ant = 0;
int namesShowed = 0;

unsigned long currentTime = 0;
unsigned long prevTime = 0;
unsigned int period = 2000;

String names[5] = {"Isaac Gallegos", "Brayan Garcia", "Yahir Alexander", "Adrian Ramirez", "Oscar Pasillas"};

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
}


void loop() {
  currentTime = millis();
  bool isExecuteTime = (currentTime - prevTime) > period;
  humi_act = dht.readHumidity();
  temp_act = dht.readTemperature();


  if (namesShowed >= (sizeof(names) / sizeof(names[0]))) {
    if (isnan(humi_act) || isnan(temp_act)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      lcd.setCursor(0, 0);
      lcd.print(" Failed to read ");
      lcd.setCursor(0, 1);
      lcd.print("from DHT sensor!");
    } else if ((humi_act != humi_ant || temp_act != temp_ant) && isExecuteTime) {
      lcd.clear();
      datos = String(humi_act) + "!" + String(temp_act);
      lcd.setCursor(0, 0);
      lcd.print("Humi: " + String(humi_act) + "%");
      lcd.setCursor(0, 1);
      lcd.print("Temp: " + String(temp_act) + "C");
      Serial.println(datos);
      humi_ant = humi_act;
      temp_ant = temp_act;
      datos = "";
      prevTime = currentTime;
    }
  } else {
    if (isExecuteTime) {
      lcd.clear();
      Serial.println(names[namesShowed]);
      lcd.setCursor(0, 0);
      lcd.print(names[namesShowed]);
      namesShowed ++;
      prevTime = currentTime;
    }
  }
}
