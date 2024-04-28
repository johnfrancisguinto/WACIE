#include <ESP32_Servo.h>
#include "BluetoothSerial.h"

volatile unsigned long startTime;
volatile unsigned long endTime;
volatile unsigned long Tap_time;

BluetoothSerial SerialBT;
Servo myservo;

String MACadd = "70:B8:F6:5B:2B:56";// Write Receiver side MAC address
uint8_t address[6]  = {0x70, 0xB8, 0xF6, 0x5B, 0x2B, 0x56};//Write Receiver side MAC address in HEX

bool connected;

String gotRequest;
bool sendReply = false;
long randomNum;
unsigned long reconnectTimer;

TaskHandle_t SerialMonitor;

volatile unsigned long val[151];
volatile unsigned long data1[151];
volatile unsigned long data2[151];
volatile unsigned int index_val = 0;

bool foundSignal = false;
bool GotSample = false;
bool GotReq1 = false;
bool GotReq2 = false;
String Command;

#define servoTap 0
#define servoRest 180
#define servoPin 25

#define signalIN 35

#define Graph1 27
#define Graph2 14
#define Button 12

void startMeasure(){
  endTime = micros();
  detachInterrupt(32);
  foundSignal = true;
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("wacieTX", true); 
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
  connected = SerialBT.connect(address);
  if(connected) {
    Serial.println("Connected Succesfully!");
    digitalWrite(15,HIGH);
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
  
  reconnectTimer = millis();
  
  pinMode(signalIN, INPUT);
  analogReadResolution(8);
  analogSetWidth(10);
  analogSetClockDiv(1);
  analogSetAttenuation(ADC_11db);
  analogSetPinAttenuation(signalIN, ADC_2_5db);
  
  pinMode(Graph1, INPUT_PULLUP);
  pinMode(Graph2, INPUT_PULLUP);
  pinMode(Button, INPUT_PULLUP);

  myservo.attach(servoPin);
  myservo.write(servoRest);


  attachInterrupt(32, startMeasure, RISING);
  
  xTaskCreatePinnedToCore(Serial_Routine, "Serial Monitor", 5000, NULL, 1, &SerialMonitor, 0);     // Default loop runs on Core 1
  delay(500); 
}

void loop() {
  if((millis() - reconnectTimer) > 5000){
    reconnectTimer = millis();
    SerialBT.connect();
  }
  
  if(!digitalRead(Button)){
    startTime = micros();
    myservo.write(servoTap);
    delay(700);
    myservo.write(servoRest);
  }

  while(SerialBT.available()) {
    gotRequest = SerialBT.readStringUntil('\n');
    Serial.println(gotRequest);
    if(gotRequest == "Req1"){
      GotReq1 = true;
      sendReply = true;
    }
    else if(gotRequest == "Req2"){
      GotReq2 = true;
      sendReply = true;
    } 
  }
  
  if(GotReq1){
    delay(100);
    GotReq1 = false;
    for(int i = 0; i < 151; i++){
      SerialBT.println(String(i) + " " + String(data1[i]));
    }
  }
  
  if(GotReq2){
    delay(100);
    GotReq2 = false;
    for(int i = 0; i < 151; i++){
      SerialBT.println(String(i) + " " + String(data2[i]));
    }
  }
}
