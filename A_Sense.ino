

float get_voltage(){

  float samples = 0;

  for(int i = 0; i < SAMPLE_COUNT; i++){
    samples += analogRead(IGN_INPUT);
    delay(SAMPLE_DELAY);
  }

  // Avarage
  samples /= (float)SAMPLE_COUNT;

  // Convert To Voltage
  samples = samples * (5.0 / 1023.0);

  // Devide with voltage divider value
  //               R2  (R1 + R2) - Correction
  return (samples * 10.9) + ADC_CORRECTION;
}


int ain(){
  return analogRead(IGN_INPUT);
}



void measurement(){
  //neo(BLUE);
  current_voltage = get_voltage();

  // Switch OFF Ignition
  if(current_voltage < LOGIC_LOW_VOLTAGE){
    if(!current_ignition) { ign_counter = 0; return; } 
    ign_counter++;
    if(ign_counter >= IGN_COUNTER_OFFSET)
    {
      current_ignition = OFF;
      wdt_counter = 0;
      need_tick = false;
    }  
  }

  // Switch ON Ignition
  else if(current_voltage > LOGIC_HIGH_VOLTAGE){
    if(current_ignition) { ign_counter = 0; return; }  
    ign_counter++; 
    if(ign_counter >= IGN_COUNTER_OFFSET)
    {
      current_ignition = ON;
      wdt_counter = 0;
      need_tick = false;
    }  
  }
  else{
    ign_counter = 0;
  }

  
}


void INIT_SENSE(){

  pinMode(IGN_INPUT, INPUT);
  
}
