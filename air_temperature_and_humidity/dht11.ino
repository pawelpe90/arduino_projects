#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#define DHT_DPIN A0
#define DHTTYPE DHT11

DHT dht(DHT_DPIN,DHTTYPE);
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(300);//Let system settle
  //Serial.println("Humidity and temperature\n\n");
  delay(700);
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp & Humidity");

}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.setCursor(5, 1);
  lcd.print("C");
  lcd.setCursor(8, 1);
  lcd.print(hum);
  lcd.setCursor(13, 1);
  lcd.print("%");
  
  //Serial.print("Temp: ");
  //Serial.print(temp);
  //Serial.print("*C; ");
  //Serial.print("Humidity: ");
  //Serial.print(hum);
  //Serial.println("%");
  
  delay(5000);

}
