#include <ESP32_Servo.h>

Servo myservo;

TaskHandle_t SerialMonitor;

volatile int val[200];
volatile int data1[200];
volatile int data2[200];
volatile int index_val = 0;
bool foundSignal = false;
bool GotSample = false;
bool GotReq1 = false;
bool GotReq2 = false;
String Command;

#define servoTap 0
#define servoRest 180
#define servoPin 13

#define signalIN 35

#define Graph1 27
#define Graph2 14
#define Button 12

void startMeasure(){
  detachInterrupt(32);
  foundSignal = true;
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  if(!digitalRead(Button)){
    myservo.write(servoTap);
    delay(700);
    myservo.write(servoRest);
    }
}
