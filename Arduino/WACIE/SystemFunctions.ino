 void Serial_Routine( void * pvParameters ){
  for(;;){
    while(foundSignal){
    val[index_val] = analogRead(signalIN);
    index_val++;
  
    if(index_val >= 150){
      index_val = 0;
      Tap_time = endTime - startTime;
      val[150] = Tap_time;
      delay(1500);
      for(int i = 0; i < 151; i++){
        if(!digitalRead(Graph1)){ //store on data1
          data1[i] = val[i];
          }
          else if(!digitalRead(Graph2)){ //store on data2
          data2[i] = val[i];
          }
          else{
            Serial.println("No storage selected");
            }
        Serial.println(String(val[i]));
        }
      foundSignal = false;
      attachInterrupt(32, startMeasure, RISING);
    }
  }    
    delay(1);
    }  
}
