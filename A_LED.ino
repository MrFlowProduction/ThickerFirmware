

// Pre-defined colors
const byte colors[7][3] = {
  { NEO_BRIGHT,  0,    0   },
  { 0,    NEO_BRIGHT,  0   },
  { 0,    0,    NEO_BRIGHT },
  { NEO_BRIGHT,  NEO_BRIGHT,  0   },
  { 0,    NEO_BRIGHT,  NEO_BRIGHT },
  { NEO_BRIGHT,  0,    NEO_BRIGHT },
  { NEO_BRIGHT,  NEO_BRIGHT,  NEO_BRIGHT },
};


/*
 * Input a value 0 to 255 to get a color value.
 * The colours are a transition r - g - b - back to r.
 */
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return neoled.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return neoled.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return neoled.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


/*
 * Set NEO Pixel color by color Index
 * Use Enums to colorset
 */
void neo(byte colorindex){

  if(neo_current_color == colorindex){return;}

  if(colorindex == 0){
    neoled.setPixelColor(0, neoled.Color(0,0,0));
  }
  else{
    colorindex--;
    neoled.setPixelColor(0, neoled.Color(
    colors[colorindex][0],
    colors[colorindex][1],
    colors[colorindex][2]));
  }

  neoled.show();
  
}


void handling_led(){
  if(current_ignition){
    neo(GREEN);
  }
  else{
    neo(RED);
  }
}


/*
 * NEO Led start demo rainbow
 */
void neo_demo(){
  
  for(int i = 0; i<256; i++){
     neoled.setPixelColor(0, Wheel(i));
     neoled.show();
     delay(15);
  }
  
}


void smooth_light(){

  for(int i = 0; i < NEO_BRIGHT; i++){
     neoled.setPixelColor(0, neoled.Color(
        0,
        0,
        i));
     neoled.show();
     delay(10);
  }
  
}


/*
 * LED will blink x times with got color index
 */
void run_blink(byte count, byte color, int interval){

  for(byte i = 0; i < count; i++){
    neo(color);
    delay(interval);
    neo(BLANK);   
    delay(interval);
  }
  
}


/*
 * You can morse a number with RGB Light
 */
void morse_number_with_led(float value){

  delay(1000);

  if(value == 0) {
    run_blink(1, LIGHT_BLUE, 3000);
    return;
  }

  String ftext = String(value, 5);

  byte digit;

  for(byte i = 0; i < ftext.length(); i++){
      
      if(ftext.charAt(i) == '.') {
        run_blink(1, WHITE, 2000);
        continue;
      }

     digit = (byte)String(ftext.charAt(i)).toInt();
     
     if(digit == 0){ 
        run_blink(1, LIGHT_BLUE, 700);
        run_blink(1, PURPLE, 1200);
     }
     else{
        run_blink(digit, BLUE, 700);
        run_blink(1, PURPLE, 1200);
     }
     
     delay(2000);
  }
}



/*
 * Simple blinker
 */
void blink_led(byte color){
  blink = !blink;

  if(blink) neo(color);
  else neo(BLANK);
}


void INIT_LED(){
  neoled.begin();
  //smooth_light();
  neo(PURPLE);
  delay(100);
}
