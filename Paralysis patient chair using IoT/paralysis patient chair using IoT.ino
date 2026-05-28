#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal.h>

Adafruit_MPU6050 mpu;

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL3k-2QSHbg"
#define BLYNK_DEVICE_NAME "Paralysis"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7
//#define USE_ESP32C3_DEV_MODULE
//#define USE_ESP32S2_DEV_KIT

#include "BlynkEdgent.h"

#define TEMP 36
#define BUZZER 12
#define LED 13
#define PulseSensorPurplePin  0       // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
//int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.

//int pinValue1;
//int pinValue;
void setup()
{
  Serial.begin(115200);
  delay(100);

  lcd.begin(16, 2);
  lcd.print("Tanmay");
  delay(2000);
  lcd.clear();
  pinMode(TEMP, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED,OUTPUT);

  BlynkEdgent.begin();

  Serial.println("Adafruit MPU6050");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  BPM_sensor();
  Temperature();
  gyroscope();
  BlynkEdgent.run();
  
}

void BPM_sensor()
{
  lcd.setCursor(1,0);
   int BPM = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                               // Assign this value to the "Signal" variable.
   Serial.println(BPM);                    // Send the Signal value to Serial Plotter.

   if(BPM > Threshold)
     {                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
        lcd.print("Heart rate:");
        lcd.print(BPM);
        digitalWrite(LED,HIGH);
        tone(BUZZER,1000,500);
     } 
   else 
     {
        lcd.print("Heart rate:");
        lcd.print(BPM);
        digitalWrite(LED,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
        noTone(BUZZER);
     }
  delay(10);
  Blynk.virtualWrite(V0,BPM);
}

void Temperature()
{
  int temp=(analogRead(A0)*0.48828125); 
  Serial.print("temperature:");
  Serial.print(temp);
  Serial.println("");
  if(temp>90)
  {
    tone(BUZZER,2000,500);
  }
  else
  {
    noTone(BUZZER);
  }
  Blynk.virtualWrite(V1,temp);
}

void gyroscope()
{
  lcd.setCursor(0,1);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  int gyro;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

  if((g.gyro.y>5) && (g.gyro.y<=15))
  { 
    lcd.print("I am Hungry");
    lcd.scrollDisplayLeft();
    //gyro=50;
  }  
 else if((g.gyro.y <-5) && (g.gyro.y>=-15))
  {
    lcd.print("I am Thirsty");
    lcd.scrollDisplayLeft();
    //gyro=100;
  }
   else if((g.gyro.x>5) && (g.gyro.x<=15))
    {
      lcd.print("I want to go Washroom");
      lcd.scrollDisplayLeft();
      //gyro=150;
    }
   else if((g.gyro.x <-5) && (g.gyro.x>=-15))
    {
      lcd.print("I wanted to talk with someone");
      lcd.scrollDisplayLeft();
      //gyro=200;
    }
    else
    {
       lcd.print("Not Recognized");
       lcd.scrollDisplayLeft(); 
       //gyro=250; 
    }
}
