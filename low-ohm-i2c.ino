//https://rcl-radio.ru/?p=45805
#include <Wire.h>
#include <Adafruit_ADS1015.h>//https://github.com/adafruit/Adafruit_ADS1X15/archive/master.zip
//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
Adafruit_ADS1115 ads;//(0x48);
/* в файле библиотеки Adafruit_ADS1015.cpp найти все строчки: ADS1015_REG_CONFIG_DR_1600SPS и заменить на: ADS1015_REG_CONFIG_DR_128SPS */
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);// RS,E,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27, 16, 2); 
float multiplier = 0.0078125;
const int u_pogr=10;       // 10 мВ - погрешность ADS1115 при измерении 5 В, // 10 mV - eroare ADS1115 la măsurarea 5 V 
const float r_ogr=51.19;  // эталонное сопротивление 100 Ом // rezistență de referință 100 Ohm 
float u3300;
int  u_digital;
byte n,k;
long u_sum,u_an,x;
float u_iz,rx,rx_m;
 
void setup(void){Serial.begin(9600);
//lcd.begin(16, 2);
lcd.begin();
lcd.backlight();
ads.begin();
delay(1000);}
 
void loop(){
  while(x<50){
x++;ads.setGain(GAIN_TWOTHIRDS);u3300=ads.readADC_SingleEnded(2)*0.1875-u_pogr;}
k++;if(k==60){x=0;k=0;}  // измерение напряжения 5В каждую минуту
 
if(n==0){ads.setGain(GAIN_SIXTEEN);multiplier = 0.0078125;}
if(u_iz>250&&n==0){n=1;}
if(n==1){ads.setGain(GAIN_EIGHT);multiplier = 0.015625;}
if(u_iz>500&&n==1){n=2;}
if(n==2){ads.setGain(GAIN_FOUR);multiplier = 0.03125;}
 
if(u_iz<500&&n==2){n=1;}
if(u_iz<250&&n==1){n=0;}
 
lcd.setCursor(1,0);
if(rx<1){rx_m=rx*1000;lcd.print("R = ");lcd.print(rx_m,1);lcd.print(" mOm");}
if(rx>=1&&rx<20){lcd.print("R = ");lcd.print(rx,3);lcd.print(" Om");}
if(rx>=20){lcd.print("  Rx > 20 Om    ");}
lcd.setCursor(1,1);lcd.print("U = ");lcd.print(u3300,1);lcd.print(" mV");
u_sum=0;
for(int i=0;i<100;i++){  // измерение
u_digital = ads.readADC_Differential_0_1();
u_sum = u_sum + u_digital;
delay(1);
}
u_an=u_sum/100;
u_iz=u_an * multiplier;
rx=(u3300/((u3300-u_iz)/r_ogr))-r_ogr;
 
lcd.clear();
}
