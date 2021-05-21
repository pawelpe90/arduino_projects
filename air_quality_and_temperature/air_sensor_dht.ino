
#include "Seeed_HM330X.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#ifdef  ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL_OUTPUT SerialUSB
#else
#define SERIAL_OUTPUT Serial
#endif

#define DHT_DPIN A0
#define DHTTYPE DHT11

HM330X sensor;
LiquidCrystal_I2C lcd(0x27,20,4);
DHT dht(DHT_DPIN,DHTTYPE);
u8 buf[30];

const char *str[]={"No", "PM1.0 ug/m3): ",
                   "PM2.5 ug/m3): ",
                   "PM10  ug/m3): ",
                   "PM1.0 ug/m3): ",
                   "PM2.5 ug/m3): ",
                   "PM10  ug/m3): ",
};
int pm_010 = 0;
int pm_025 = 0;
int pm_100 = 0;
int pm_025_perc = 0.0;
int pm_100_perc = 0.0;

HM330XErrorCode print_result(const char* str,u16 value)
{
    if(NULL==str)
        return ERROR_PARAM;
    SERIAL_OUTPUT.print(str);
    SERIAL_OUTPUT.println(value);
    return NO_ERROR;
}

/*parse buf with 29 u8-data*/
HM330XErrorCode parse_result(u8 *data)
{
    u16 value=0;
    if(NULL==data)
        return ERROR_PARAM;
        
    pm_010 = (u16)data[4]<<8|data[5];
    pm_025 = (u16)data[6]<<8|data[7];
    pm_100 = (u16)data[8]<<8|data[9];

    pm_025_perc = (float)pm_025/25*100;
    pm_100_perc = (float)pm_100/50*100;

    return NO_ERROR;
}

HM330XErrorCode parse_result_value(u8 *data)
{
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=0;i<28;i++)
    {
        SERIAL_OUTPUT.print(data[i],HEX);
        SERIAL_OUTPUT.print("  ");
        if((0==(i)%5)||(0==i))
        {
            SERIAL_OUTPUT.println(" ");
        }
    }
    u8 sum=0;
    for(int i=0;i<28;i++)
    {
        sum+=data[i];
    }
    if(sum!=data[28])
    {
        SERIAL_OUTPUT.println("wrong checkSum!!!!");
    }
    SERIAL_OUTPUT.println(" ");
    SERIAL_OUTPUT.println(" ");
    return NO_ERROR;
}


/*30s*/
void setup()
{
    SERIAL_OUTPUT.begin(115200);
    delay(100);
    SERIAL_OUTPUT.println("Serial start");
    if(sensor.init())
    {
        SERIAL_OUTPUT.println("HM330X init failed!!!");
        while(1);
    }
  dht.begin();
	lcd.begin(16,2);
	lcd.backlight();

}



void loop()
{
    if(sensor.read_sensor_value(buf,29))
    {
        SERIAL_OUTPUT.println("HM330X read result failed!!!");
    }
    parse_result_value(buf);
    parse_result(buf);

    float temp = dht.readTemperature();
    int hum = dht.readHumidity();
	
	  lcd.setCursor(0, 0);
	  lcd.print("PM 2.5: ");
	  lcd.print(pm_025);
    lcd.print(" ");
    lcd.print(pm_025_perc);
    lcd.print("%");
	  lcd.setCursor(0, 1);
	  lcd.print("T: ");
	  lcd.print(temp);
    lcd.print("C");
    lcd.print(" H: ");
    lcd.print(hum);
    lcd.print("%");
//    SERIAL_OUTPUT.print("PM 1: ");
//    SERIAL_OUTPUT.println(pm_1);
//    SERIAL_OUTPUT.print("PM 2.5: ");
//    SERIAL_OUTPUT.println(pm_2_5);
//    SERIAL_OUTPUT.print("PM 10: ");
//    SERIAL_OUTPUT.println(pm_10);
    delay(2000);
}
