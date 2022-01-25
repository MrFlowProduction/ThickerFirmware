/*
   ########################################
   #                                      #
   #        Thicker Firmware V1.1         #
   #            by Mr.Flow                #
   #                                      #
   ########################################

   for Ignition simulator project

   Programmer:
   Florian Szekely
   florian.szekely@aseso.hu
   +36 30 556 2270


   Supported Products:
   - Micronet A317
   - Micronet Treq-317
   - Micronet SmartTab 8

  Input voltage rating: 6.0 - 36VDC
   

*/


// ___________________________ PINOUT ___________________________________
// =======================================================================
//      NAME              PIN          DESCRIPTION
//========================================================================
#define NEO_LED           2         // Neo Pixel led output
#define IGN_INPUT         A3        // Ignition analog input
#define IGN_OUTPUT        4         // Ignition digital output
#define BTN_INPUT         0         // Function Button
//========================================================================


// ############################### Colors <<<<
#define BLANK             0
#define RED               1
#define GREEN             2
#define BLUE              3
#define ORANGE            4
#define LIGHT_BLUE        5
#define PURPLE            6
#define WHITE             7


// Enums
#define ON      1
#define OFF     0
#define ENABLE  1
#define DISABLE 0

// ------------------- Includes --------------------
#include <Adafruit_NeoPixel.h>  // Adafruit Neopixel led
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>


Adafruit_NeoPixel   neoled = Adafruit_NeoPixel(1, NEO_LED, NEO_GRB + NEO_KHZ800);


#define INTERRUPT_PIN PCINT0  // This is PB0 per the schematic
#define INT_PIN PB0           // Interrupt pin of choice: PB0 (same as PCINT0) - Pin 5
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarit



/// __________________________________ Configuration ____________________________________________
// =============================================================================================
//      NAME                  VALUE                    DESCRIPTION
// =============================================================================================
#define NEO_BRIGHT            150                   // NEO led default bright
#define SAMPLE_COUNT          10                    // Ignition voltage sample count per read
#define SAMPLE_DELAY          5                     // (ms) Ignition delay between samples
#define MEAS_INTERVAL         70                    // (ms) Ignition measurement interval
#define LOGIC_LOW_VOLTAGE     25                    // (V) Ignition logic low voltage 
#define LOGIC_HIGH_VOLTAGE    28                    // (V) Ignition logic high voltage
#define ADC_CORRECTION        0                     // (V) Correction value
#define TICK_DELAY            1000                  // (ms) Tick delay
#define TICK_INTERVAL         900                   // (sec) Tick interval - time between ticks
#define IGN_COUNTER_OFFSET    8                     // Ignition switching offset (cycle count before change output)
// =============================================================================================




// ____________________________________________ Globals ___________________________________________________
// ========================================================================================================
// TYPE               NAME                  DEF. VALUE         DESCRIPTION
// ========================================================================================================
static byte           neo_current_color =   255;            // Neoled current color
static float          current_voltage   =   0;              // Current measured voltage
static bool           current_ignition  =   false;          // Current ignition State
static bool           need_tick         =   false;          // Need ignition tick
static uint16_t       wdt_counter       =   0;              // Watchdog cycle counter
static uint16_t       wdt_count         =   0;              // Watchdog calculated count
volatile bool         isButtonPressed   = false;            // Detect button pressed state
volatile bool         isButtonHandled   = true;             // Button handling flag
bool                  test_mode         = false;            // Test mode flag
byte                  ign_counter       =   0;              // Ignition cicle counter
// =========================================================================================================


// WDR Interrupt
ISR(WDT_vect) {

  if(!need_tick && !current_ignition){   
    if(wdt_counter++ >= wdt_count){
      need_tick = true;
    }  
  }
}

// Button Interrupt
ISR(PCINT_VECTOR)
{
 
  if(isButtonHandled){
    if(!buttonState()) return;
    isButtonPressed = true;
    isButtonHandled = false;
  }
  
}


// Quick Test for Ignition signal
void quick_test(){
  if(isButtonPressed){
      isButtonHandled = true;
      isButtonPressed = false;
      need_tick = true;
    }
}


void test(){

  bool mode = 0;
  int sample = 0;

  neo_demo();
  
  while(true){

    if(isButtonPressed && !isButtonHandled){
      isButtonPressed = false;
      isButtonHandled = true;
      mode = !mode;
    }

    if(mode){
      sample = (int)(get_voltage());
      sample = map(sample, 0, 35, 0, 150);      
      neoled.setPixelColor(0, Wheel(sample));
      neoled.show();
    }
    else{
      neo(BLANK);    
      set_output(ON);
      delay(1000);
      set_output(OFF);
      delay(1000);
    }
  } 
}



void setup() {
  INIT_LED();
  INIT_SENSE();
  INIT_IGNITION();
  INIT_WDT();
  INIT_BUTTON();
  //test();
}

void loop() {

  if(test_mode){
    test();
  }
  else{
    measurement();
    drive_output();
    handling_led(); 
    delay(MEAS_INTERVAL);
    quick_test();
  }
  
}
