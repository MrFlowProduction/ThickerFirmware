
/*
 * Set Ignition Output
 */
void set_output(bool state){
  
  if(state == digitalRead(IGN_OUTPUT)){
    return;
  }

  // NPN Drive output
  digitalWrite(IGN_OUTPUT, state);
}


/*
 * Ignition Alive Tick
 */
void tick(){
  neo(WHITE);
  set_output(ON);
  delay(TICK_DELAY);
  set_output(OFF);
}


/*
 * Drive Ignition output by current voltage
 */
void drive_output(){

  // Set ignition output
  set_output(current_ignition);

  // Handling ticks
  if(!current_ignition && need_tick){

    // Ignition tick
    tick(); 
    
    // ACK for watchdog
    need_tick = false;
    wdt_counter = 0;
  }
  
}



void INIT_IGNITION(){

  pinMode(IGN_OUTPUT, OUTPUT);
  digitalWrite(IGN_OUTPUT, LOW);
}
