/*
   ########################################
   #                                      #
   #        Thicker Firmware V1.3         #
   #            by Mr.Flow                #
   #                                      #
   ########################################

   for Ignition simulator project

   Programmer:
   Florian Szekely
   florian@mrflow.hu
   +36 30 097 3366


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
#include <EEPROM.h>


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
#define ADC_CORRECTION        0                     // (V) Correction value
#define TICK_DELAY            1000                  // (ms) Tick delay
#define TICK_INTERVAL         900                   // (sec) Tick interval - time between ticks
#define IGN_COUNTER_OFFSET    8                     // Ignition switching offset (cycle count before change output)
#define START_DELAY           2000                  // (ms) System start delay
#define START_DELAY           500                   // (ms) System start delay
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
bool                  setup_mode        =   false;          // Setup mode if no logic values
byte                  ign_counter       =   0;              // Ignition cicle counter
float                 LOGIC_LOW_VOLTAGE =   23.0;           // (V) Ignition logic low voltage 
float                 LOGIC_HIGH_VOLTAGE =  26.0;           // (V) Ignition logic high voltage
bool                  blink             = false;            // Blink helper variable
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
  int counter = 0;

  neo_demo();
  
  while(true){

    if(isButtonPressed && !isButtonHandled){
      isButtonPressed = false;
      isButtonHandled = true;
      mode = !mode;

      // Memória beállítás bekapcsolása
      if(counter++ >= 10){
        setMemOk(0);
        neo(ORANGE);
        setup_mode = true;
        test_mode = false;
        break;
      }
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


/* Memórában tárolt beállítások setelése */
void setupper(){

  bool setLow = true;

  float low, high;
  
  while(true){

    
    // Blinking
    if(setLow){
      blink_led(RED);
    }
    else{
      blink_led(GREEN);
    }


    if(isButtonPressed && !isButtonHandled){
      isButtonPressed = false;
      isButtonHandled = true;

      if(setLow){
        low = get_voltage();
        setLow = false;
      }
      else{
        high = get_voltage();
        break;
      }
    }
    
    delay(500);
  }


  // Blinking the setted voltages
  neo(ORANGE);
  delay(10000);
  neo(BLANK);   
  delay(2000);

  morse_number_with_led(low);
  
  neo(GREEN);
  delay(5000);
  neo(BLANK);   
  delay(2000);
  
  morse_number_with_led(high);


  // Save voltages
  mem_set(low, high);
  LOGIC_LOW_VOLTAGE = low;
  LOGIC_HIGH_VOLTAGE = high;
  setup_mode = false;
}



void dev(){

  while(true){
    morse_number_with_led(109.20482);
    run_blink(1, GREEN, 2000);
    morse_number_with_led(22.17);
  }
  
}



void setup() {
  INIT_LED();
  INIT_SENSE();
  INIT_IGNITION();
  INIT_WDT();
  INIT_BUTTON();
  INIT_MEMORY();
  delay(START_DELAY);
}

void loop() {

  // ONLY For development!!!
  // setup_mode = true;

  if(test_mode){
    test();
  }
  else if(setup_mode){
    setupper();
  }
  else{
    measurement();
    drive_output();
    handling_led(); 
    delay(MEAS_INTERVAL);
    quick_test();
  }
  
}
