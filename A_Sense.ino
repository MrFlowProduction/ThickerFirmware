

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
  return (samples / (22 / 142.0)) + ADC_CORRECTION;
}



void measurement(){
  neo(BLUE);
  current_voltage = get_voltage();

  // Switch OFF Ignition
  if(current_voltage < LOGIC_LOW_VOLTAGE){
    current_ignition = OFF;
  }

  // Switch ON Ignition
  else if(current_voltage > LOGIC_HIGH_VOLTAGE){
    current_ignition = ON;
  }
}


void INIT_SENSE(){

  pinMode(IGN_INPUT, INPUT);
  
}
