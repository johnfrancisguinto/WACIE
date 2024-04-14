 void Serial_Routine( void * pvParameters ){
  for(;;){
    while(foundSignal){
    val[index_val] = analogRead(signalIN);
    index_val++;
    if(index_val >= 150){
      index_val = 0;
      delay(1500);
      for(int i = 0; i < 150; i++){
        if(!digitalRead(Graph1)){ //store on data1
          data1[i] = val[i];
          }
          else if(!digitalRead(Graph2)){ //store on data2
          data2[i] = val[i];
          }
          else{
//            Serial.println("No storage selected");
            }
        Serial.println(String(val[i]));
        }
      foundSignal = false;
      attachInterrupt(32, startMeasure, RISING);
    }
  }
  
    if (Serial.available()) {      // If anything comes in Serial (USB),
      Command = Serial.readStringUntil('\n');
      if(Command == "Req1"){
        GotReq1 = true;
        }
      if(Command == "Req2"){
        GotReq2 = true;
        }
    }
    
    if(GotReq1){
        GotReq1 = false;
        for(int i = 0; i < 150; i++){
          Serial.println(String(i) + " " + String(data1[i]));
          }
      }
      
    if(GotReq2){
        GotReq2 = false;
        for(int i = 0; i < 150; i++){
          Serial.println(String(i) + " " + String(data2[i]));
          }
      }
      
    delay(1);
    }  
}
