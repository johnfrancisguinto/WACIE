#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String gotData[200];
String RequestData;
int dataCount = 0;
bool allData = false;
unsigned long replyTimer;
long timeout = 3000;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  
  if (Serial.available()) {
    RequestData = "";
    RequestData = Serial.readStringUntil('\n');
    replyTimer = millis();
    if(RequestData == "Req1"){
      dataCount = 0;
      SerialBT.print("Req1\n");
      }
    if(RequestData == "Req2"){
      dataCount = 0;
      SerialBT.print("Req2\n");
      } 
    SerialBT.flush();
  }
  while(SerialBT.available()) {
  gotData[dataCount++] = SerialBT.readStringUntil('\n');
  }
  
  if(dataCount == 150 && ((millis() - replyTimer) < timeout)){
    for(int i = 0; i<dataCount; i++){
      Serial.println(String(i) + " " + String(gotData[i]));
      }
    dataCount = 0;
  }
  else if((millis() - replyTimer) > timeout){
    replyTimer = millis();
    if(RequestData == "Req1"){
      dataCount = 0;
      SerialBT.print("Req1\n");
      }
    if(RequestData == "Req2"){
      dataCount = 0;
      SerialBT.print("Req2\n");
      } 
    SerialBT.flush();
    }
}
