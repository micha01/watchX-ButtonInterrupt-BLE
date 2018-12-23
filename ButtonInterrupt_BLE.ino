
#include "PinChangeInterrupt.h"
//#include "Adafruit_BLE.h"   // uncommenting will cause error

#include <Adafruit_GFX.h>                 
#include <Adafruit_SSD1306.h> 
#include <avr/sleep.h>
#include <Wire.h>
#include <MPU6050.h>
// Uncomment this block to use hardware SPI
#define OLED_DC     A3
#define OLED_CS     A5
#define OLED_RESET  A4

#define INT6_PIN 7
#define BUTTON1 8
#define BUTTON2 11
#define BUTTON3 10
#define buzzer 9

#define R1 10000
#define R2 10000
#define VOLTAGEDIV 0.5
#define BATTERYENERGY 4
#define BATTERYINPUT A11

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
MPU6050 mpu;

float batteryLevel=0;
float usbbatteryLevel=0;

int interruptCount=0;
int count=0;
void interruptFunction() {
 interruptCount++;
 display.ssd1306_command(SSD1306_DISPLAYON);
}


void setup() {
 display.begin(SSD1306_SWITCHCAPVCC);
 pinMode(BUTTON1, INPUT_PULLUP);  // left up
 pinMode(BUTTON2, INPUT_PULLUP);  // right up
 pinMode(BUTTON3, INPUT_PULLUP);  // right down
 
 mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G); 
 mpu.setSleepEnabled(true);
 
 display.setTextSize(2);
 display.setTextColor(WHITE);
}



void loop() {
 display.clearDisplay();
 showBattery();

  // BUTTON1 pressed
  if (digitalRead(BUTTON1) == 0){
    tone(buzzer,3000,100);
    delay(1000);
  }
  // BUTTON2 pressed
  if (digitalRead(BUTTON2) == 0 ){
    tone(buzzer,3000,100);
    delay(500);     
    sleepNow();    
  }
  // BUTTON3 pressed
  if (digitalRead(BUTTON3) == 0 ){
    tone(buzzer,3000,100); 
  }

 display.display();  
}


void showBattery(){
 digitalWrite(BATTERYENERGY, HIGH);
 delay(50);
 float voltage = analogRead(BATTERYINPUT);
 voltage = (voltage / 1024) * 3.35;
 voltage = voltage / VOLTAGEDIV;
 delay(50);
 digitalWrite(BATTERYENERGY, LOW);
 batteryLevel = (voltage - 3.38) / 0.0084;
 display.setCursor(0, 20);
 display.print("Bat:");
 display.print((int) batteryLevel);
 display.print("%"); 
}

void sleepNow()        
{
   display.ssd1306_command(SSD1306_DISPLAYOFF);
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
   sleep_enable();         
   attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(BUTTON1), interruptFunction, FALLING);
   sleep_mode();                  
   sleep_disable();             
   detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(BUTTON1));
}
