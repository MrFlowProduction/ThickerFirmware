# ThickerFirmware
 
Base on Arduino IDE
Minimum 1.8.15

https://www.arduino.cc/en/software

## Board - Attiny85

https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

## Libraries

https://github.com/adafruit/Adafruit_NeoPixel

<avr/wdt.h>

History:
V1.3 - 2022.10.17:
- EEPROM kezelés kiiktatva a további meghibásodások elkerülése végett
- Statikus gyújtás érzékelés beállítások (LOGIC_LOW_VOLTAGE, LOGIC_HIGH_VOLTAGE)
- LOGIC_LOW_VOLTAGE =   23.0
- LOGIC_HIGH_VOLTAGE =  26.0;
- Rejtett funkció: RGB LED használata float számok kiíratására
