// This is only a "run once" file that was used to store the food items in the EEPROM
// and it doesn't play any further part on the execution of the project. 

#include <EEPROM.h>

// In the setup we call add_item() for the food we want to add only once and then it is stored forever in the EEPROM.
// The food items along with their values have been precalcalculated and stored in the EEPROM.
void setup() {
  Serial.begin(300);
  //add_item("Pastitsio", 0, 0);
  //add_item("Mpamies", 90, 11);
  check_item(1);
}

// No loop needed, as the population will take place once
void loop() {}

void check_item(int pos) {
  uint8_t val = EEPROM.read(pos);
  if(val > 5) {
    Serial.println("WRONG POSITION");
    return;
  }
  
  Serial.println(val);

  int i = 0;
  char c;
  while((c = EEPROM.read(pos+1+i)) != '\0') {
    Serial.print(c);
    ++i;
  }

}

// A function that adds a food to the EEPROM along with its val (0, 255)
void add_item(const char* food, uint8_t val, int pos) {
  EEPROM.write(pos, val); 
  
  int i = -1;
  do {
    ++i;
    EEPROM.write(pos+1+i, (int)food[i]);
  } while (food[i] != '\0'); // null terminated string
}
