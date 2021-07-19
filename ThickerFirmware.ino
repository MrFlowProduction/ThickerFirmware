
/*
   ########################################
   #                                      #
   #        Thicker Firmware V1.0         #
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
#define NEO_LED           0         // Neo Pixel led output
#define IGN_INPUT         A2        // Ignition analog input
#define IGN_OUTPUT        3         // Ignition digital output
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


Adafruit_NeoPixel   neoled = Adafruit_NeoPixel(1, NEO_LED, NEO_GRB + NEO_KHZ800);




/// __________________________________ Configuration ____________________________________________
// =============================================================================================
//      NAME                  VALUE                    DESCRIPTION
// =============================================================================================
#define NEO_BRIGHT            150                   // NEO led default bright
#define SAMPLE_COUNT          100                   // Ignition voltage sample count per read
#define SAMPLE_DELAY          3                     // (ms) Ignition delay between samples
#define MEAS_INTERVAL         1000                  // (ms) Ignition measurement interval
#define LOGIC_LOW_VOLTAGE     20                    // (V) Ignition logic low voltage 
#define LOGIC_HIGH_VOLTAGE    22                    // (V) Ignition logic high voltage
#define ADC_CORRECTION        0                     // (V) Correction value
#define TICK_DELAY            2000                  // (ms) Tick delay
#define TICK_INTERVAL         300                   // (sec) Tick interval - time between ticks
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
// =========================================================================================================



ISR(WDT_vect) { // WDT interrupt vector

  
  if(current_ignition){
    wdt_counter = 0;
    need_tick = false;
  }
  else if(!need_tick){
    
    if(wdt_counter++ >= wdt_count){
      need_tick = true;
    }
    
  }
  
}




void setup() {
  INIT_LED();
  INIT_SENSE();
  INIT_IGNITION();
  INIT_WDT();
}

void loop() {
  
  measurement();
  drive_output();
  handling_led();
  
  delay(MEAS_INTERVAL);
  
}
