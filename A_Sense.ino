
#define CRC_MEM_S       0xFA
#define CRC_MEM_E       0xCF
#define CRC_MEM_S_ADDR  1
#define CRC_MEM_E_ADDR  99

#define MEM_OK          2
#define MEM_LOW_VOLT    5
#define MEM_HIGH_VOLT   10


/* Memory Clear */
void mem_clear(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  EEPROM.write(CRC_MEM_S_ADDR, CRC_MEM_S);
  EEPROM.write(CRC_MEM_E_ADDR, CRC_MEM_E);
}


/* Is Need memory set */
bool isMemNeedSet(){
  if(EEPROM.read(MEM_OK) == 0) return true;
  return false;
}

/* Set Need memory setup value */
bool setMemOk(byte value){
  EEPROM.update(MEM_OK, value);
}


/* Memory Check */
bool mem_check(){
  if(EEPROM.read(CRC_MEM_S_ADDR) != CRC_MEM_S) return false;
  if(EEPROM.read(CRC_MEM_E_ADDR) != CRC_MEM_E) return false;
  return true;
}


/* Get Memory Value */
float mem_get_value(int addr){

  float value = 0.0f;
  EEPROM.get(addr, value);

  return value;
}


/* Get Memory Value */
void mem_set_value(int addr, float value){
  EEPROM.put(addr, value);
}

/* Memóriában tárolt értékek beállítása */
void mem_set(float lowVolt, float highVolt){
  mem_set_value(MEM_LOW_VOLT, lowVolt);
  mem_set_value(MEM_HIGH_VOLT, highVolt);
  setMemOk(1);
}



void INIT_MEMORY(){

  setup_mode = false;
  return;

  // Ha még nem volt inicializálva a memória, akkor törli
  if(!mem_check()) mem_clear();

  // Kell-e beállítani a memóriát
  setup_mode = isMemNeedSet();

  // Ha szükséges a beállítás, akkor nincs mit kiolvasni
  if(setup_mode) return;

  LOGIC_LOW_VOLTAGE = mem_get_value(MEM_LOW_VOLT);
  LOGIC_HIGH_VOLTAGE = mem_get_value(MEM_HIGH_VOLT);
  
}
