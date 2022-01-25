

// Button event handling
void button_event(){
  isButtonPressed = true;
}


bool buttonState(){
  return !digitalRead(BTN_INPUT);
}

// INIT Function buttons
void INIT_BUTTON(){

  pinMode(BTN_INPUT, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable

  // Detect test mode
  if(buttonState()){
    test_mode = true;
  }
  
  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  sei();
}
