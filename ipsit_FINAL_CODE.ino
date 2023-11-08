#define BLYNK_TEMPLATE_ID "TMPLH0QTqrUR"
#define BLYNK_TEMPLATE_NAME "Miner Helmet"
#define BLYNK_AUTH_TOKEN "vWQWgwkNaIzCjRTYPgeDiDDzYUQrqqus"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <TinyGPS++.h>
//#include <SoftwareSerial.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "realme Gt Neo 3t";
char pass[] = "ipanu@123";

BlynkTimer timer;

#define mq9 4
#define vib 34
#define buz 22
#define button 18

//static const int RXPin = 5, TXPin = 2;  //Rx=4, Tx=5
//static const uint32_t GPSBaud = 9600;
//float latitude;
//float longitude;
//TinyGPSPlus gps;
//SoftwareSerial ss(RXPin, TXPin);

#include<DHT.h>
#define Dpin 23
#define Dtype DHT11

DHT dht(Dpin,Dtype);
 //Variables
 float t, h;
 long measurement=0;
 int data=0;

 void sendSensorDHT()
{
  h = dht.readHumidity();
  t = dht.readTemperature(); 
  if(isnan(t) || isnan(h)){
    Serial.println("Check the connection of DHT11");
    return;
  }
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);
} 
void sendSensorMQ9(){
  float analogValue = analogRead(mq9);
  float voltage = analogValue * (3.3 / 4096.0);
  float ppm = map(voltage, 0.2, 2.0, 20, 2000);
//  Serial.print("CO PPM = ");
//  Serial.println(ppm);
  Blynk.virtualWrite(V3, ppm);
}
void sendSensorVib(){
  measurement=vibration();
//  Serial.print("Vibration: ");
//  Serial.println(measurement);
  Blynk.virtualWrite(V2, measurement);
}
long vibration(){
  measurement=pulseIn(vib,HIGH);
  return measurement;
}

void sendSensorButton(){
  data=digitalRead(button);
//  Serial.println(data);
  Blynk.virtualWrite(V5,data);
}

void setup() {
  Serial.begin(9600);
  pinMode(mq9,INPUT);
  pinMode(vib,INPUT);
  pinMode(buz,OUTPUT);
  pinMode(button, INPUT_PULLUP);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensorDHT);
  timer.setInterval(1000L, sendSensorMQ9); 
  timer.setInterval(2000L, sendSensorVib);
  timer.setInterval(1000L, sendSensorButton);
  /* delay(10000);
  ss.begin(GPSBaud);*/
}
BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  
  if (pinValue == 1){
   // do something when button is pressed;
   for (int i = 0; i < 55; i++) {
    digitalWrite(buz, HIGH); // turn the buzzer on
    delay(100); // wait for 100 milliseconds
    digitalWrite(buz, LOW); // turn the buzzer off
    delay(100); // wait for 100 milliseconds
  }
   timer.setTimeout(5000L, []()
       {
        digitalWrite(buz, LOW);
        Blynk.virtualWrite(V4,0); 
       });
  }
}

void loop() {
 /* while (ss.available() > 0){
    if (gps.encode(ss.read())){
      displayInfo();
    }
  } */
  Blynk.run();
  timer.run();
}

/* void displayInfo(){
  if (gps.location.isUpdated()){
      //Serial.print("Latitude= ");
      latitude =  gps.location.lat();
      //Serial.println(latitude,6);
      //Serial.print("Longitude= "); 
      longitude = gps.location.lng();
      //Serial.println(longitude,6);


      Blynk.virtualWrite(V6, String(latitude, 6));
      Blynk.virtualWrite(V7, String(longitude, 6));
    }
} */
