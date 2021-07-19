


void INIT_WDT(){
  cli();
  WDTCR = (1 << WDCE) | (1 << WDE);

  // 8 SEC
  WDTCR = (0<<WDE) | (1<<WDIE) | (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0);
  sei();

  // Calculate interval count
  wdt_count = (uint16_t)(TICK_INTERVAL / 4);
}
