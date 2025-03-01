// Program that takes the input of a button and performs:
// - Action 1 in button push
// - Action 2 in button hold
// - Action 3 when button is not pushed

// A bitmap stored in the variable button_states is used to store the button inputs and
// determine the action to be taken.
// Each time interval (a custon time interval between button state checks can be used by
// adjustiong the delay() depending on how responsive you want your program to be, or how
// much flexibility you want to give to the end user) the state of the button is checked,
// the bitmap is shifted 1 bit to the left erasing the oldest state adn with a bitwise or
// the new state is saved in the least significant bit.

// Button press is checked on button release as to be distinguished from button hold.
// When the button is held the variable disable_button_push is set to 1, as to not
// interpret the release of the button when held with a button release meant for a press,
// it is then set back to 0 when the button is inactive.

#include "set.h"
#include "rtc.h"
#include <LiquidCrystal.h>

// Variable Declaration for button
word button_states = 0;
word button_input;
const word push_mask = 0b0000000000000011;  // to keep the last 2 bits / button inputs
byte disable_button_push = 0;

uint8_t already_recommended_num = 0;


// Food Recommender
typedef struct food_item {
    const char* food_name;
    const char* food_name_rest;
    uint8_t val;
} food_item;

food_item array[69] = {
// Meat - Summer
    { "KOLOKY8AKIA", "GEMISTA", 2 },

// Meat - FFA 
    { "PASTITSIO", NULL, 3 },
    { "MAKARONIA ME", "KIMA", 6 },
    { "KANELONIA", NULL, 7 },
    { "KREAS ME", "RYZI", 10 },
    { "GIOYBETSI", NULL, 12 },
    { "KREAS ME", "XYLOPITES", 13 },
    { "KREAS ME", "MAKARONIA", 14 },
    { "KREAS ME", "PATATES", 18 },
    { "KREAS", "LEMONATO", 21 },
    { "KREAS", "KOKKINISTO", 24 },
    { "KREAS ME", "POYRE", 26 },
    { "SYKWTI", NULL, 28 },
    { "MPIFTEKIA", NULL, 31 },
    { "KEFTEDAKIA", NULL, 32 },
    { "SPETSOFAI", NULL, 33 },
    { "SOYTZOYKAKIA", NULL, 34 },
    { "MPRIZOLES", NULL, 35 },
    { "ROLO", NULL, 36 },
    { "PAPOYTSAKIA", NULL, 37 },
    { "THGANIA", NULL, 38 },
  
// Meat - Winter
    { "STIFADO", NULL, 39 },               
    { "LAXANON", "TOLMADES", 41 },       
    { "FRIKASE", NULL, 42 },               
    { "MPLOYGOYRO", "SOYPA", 44 },       
    { "KOTOSOYPA", NULL, 45 }, 

// Non Meat - Winter
    { "QAROSOYPA", NULL, 47 },
    { "XORTOSOYPA", NULL, 51 },
    { "FASOLADA", NULL, 53 },
    { "REBY8IA", NULL, 55 },
    { "FABA", NULL, 56 },
    { "TRAXANAS", NULL, 57 },
    { "LAXANORYZO", NULL, 58 },
    { "XYLOPITES ME", "MELITZANES", 59 },

// Non Meat - FFA
    { "MANESTRA", NULL, 60 },
    { "MPAMIES", NULL, 61 },
    { "SPANAKORYZO", NULL, 63 },
    { "SOYFLE", NULL, 65 },
    { "KOLOKY8O", "KEFTEDES", 66 },
    { "MAKARONIA ME", "SALTSA", 71 },
    { "MAKARONIA ME", "KIMA", 72 },
    { "TORTELINIA", NULL, 73 },
    { "XYLOPITES", "MAKRIES", 74 },
    { "KAGIANAS", NULL, 75 },
    { "PATATO", "KROKETES", 76 },
    { "QARI PLAKI", NULL, 77 },
    { "XYLOPITES", "KONTES", 78 },
    { "FAKES", NULL, 80 },
    { "MANITARIA", "LADORIGANH", 81 },
    { "MANITARIA ALA", "KREM", 82 },
    { "TSIPOYRES", NULL, 86 },
    { "KOKKINO", "QARO", 90 },
    { "MPAKALIAROS", NULL, 93 },
    { "GALAIOS", NULL, 94 },
    { "PROSFYGAKIA", NULL, 95 },
    { "GABROS", NULL, 97 },
    { "QARAKIA", "THGANITA", 99 },
    { "GARIDES", NULL, 100 },
    { "GARIDO", "MAKARONADA", 101 },
    { "JESPYRIA", NULL, 102 },
    { "GIGANTES", NULL, 103 },
    { "MPRIAM", NULL, 108 },
    { "PATATES", "MPLOYM", 111 },
    { "ARAKAS", NULL, 114 },
    { "IMAM", "MPAILNTI", 115 },           
    { "PILAFI ME", "MANITARIA", 116 },

// Non Meat - Summer
    { "GEMISTA", NULL, 119 },                
    { "KOLOKY8O", "KORFADES", 120 },       
    { "FASOLAKIA", NULL, 122 }
};


// A set that contains all the food items that have been recommended
// in this session, as to not have repeats.
Set already_recommended;

// The 
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

// custom character 'Γ'
byte GAMMA[8] = {
  0b11111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b00000
};

// custom character 'Δ'
byte DELTA[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

// custom character 'Θ'
byte THETA[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};

// custom character 'Λ'
byte LAMDA[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b00000  
};

// custom character 'Ξ'
byte KSI[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b11111,
  0b00000
};

// custom character 'Π'
byte PIE[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b00000
};

// custom character 'Φ'
byte FI[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b10101,
  0b10101,
  0b01110,
  0b00100,
  0b00000
};

// custom character 'Ψ'
byte PSI[8] = {
  0b10101,
  0b10101,
  0b10101,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
  



void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // random
  randomSeed(datetime_seed());

  // rtc
  init_rtc("__DATE__", __TIME__);

   // custom character 'Γ'
  lcd.createChar(0, GAMMA);

  // custom character 'Δ'
  lcd.createChar(1, DELTA);

  // custom character 'Θ'
  lcd.createChar(2, THETA);

  // custom character 'Λ'
  lcd.createChar(3, LAMDA);

  // custom character 'Π'
  lcd.createChar(4, KSI);

  // custom character 'Π'
  lcd.createChar(5, PIE);

  // custom character 'Φ'
  lcd.createChar(6, FI);

  // custom character 'Ψ'
  lcd.createChar(7, PSI);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();

  print_greek("KALHMERA GIAGIA!");
}


void loop() {
    button_input = digitalRead(7);
    button_states = button_states << 1;
    button_states = button_states | button_input;

    // small delay between button positon checks to give apropriate time to the end-user
    // and not misinterpret button hold and button push
    delay(40);

    if ((button_states & push_mask) == 2 && !disable_button_push) {  // button push - Action 1
        digitalWrite(LED_BUILTIN, 1);
    
        // searching for the next food to print (next_item() might return {NULL, NULL, NULL} in which case we need to call it again)
        food_item food = {NULL, NULL, NULL};
        while(food.food_name == NULL) {
            food = next_item();
        }
        
        // printing the next item on the LCD
        lcd.clear();
        print_greek(food.food_name);
        if(food.food_name_rest != NULL) {
            lcd.setCursor(0, 1);
            print_greek(food.food_name_rest);
        }

    } 
    else if (button_states == 0b1111111111111111) {  // button hold - Action 2
        digitalWrite(LED_BUILTIN, 1);
        delay(50);
        digitalWrite(LED_BUILTIN, 0);
        disable_button_push = 1;  // makes sure that in the next cycle of the loop it does not interpret it as a button push
    } 
    else {
        digitalWrite(LED_BUILTIN, 0);
        disable_button_push = 0;
    }
}


// The next_item() function returns a random food to be displayed on the LCD
// The algorithm used picks a random integer in the range [0, 122] and this integer
// is then used to determine the next food item to be returned.
//
// Each food in the array has a value assigned to it, this value describes the
// upper bound of its range with the lower bound being the upper bound + 1 of the
// previous food item in the array.
// So the food's whose range includes this random integer is the one that will be picked.
//
// For example:
// rand_num = 7
// food0, 1: [0,1]
// food1, 3: [2,4]
// food2, 5: [5, 9]
// so the item picked is food2.
//
// That means that each item has a different probability of getting picked, which is val/123,
// meaning foods with a higher val will be picked more often.


food_item next_item() {

  // Get day and month and choose which foods are available
  // for selection by adjusting Binary search's bounds
  uint8_t day = Day();
  int8_t low, high;

  switch(Month()){
    // Winter
    case 11:
    case 12:
    case 1:
    case 2: 

      if(day == 3 || day == 5){ // non meat - winter
        low = 26;
        high = 65;
      }
      else if(day == 7){ // meat - winter
        low = 1;
        high = 25;
      }
      else{ // both - winter
        low = 1;
        high = 65;
      }
      break;

    // Summer
    case 6:
    case 7: 
    case 8: 

      if(day == 3 || day == 5){ // non meat - summer
        low = 34;
        high = 68;
      }
      else if(day == 7){ // meat - summer
        low = 0;
        high = 20;
      }
      else{ // both - summer
        // choosing 1 of the 2 groups between meat and non-meat
        //beacause the range wraps around
        if(random(0,2) == 0){
          low = 0;
          high = 20;
        }
        else{
          low = 34;
          high = 68;
        }
      }
      break;

    default: // open season

      if(day == 3 || day == 5){ // non meat - open season
        low = 26; 
        high = 68;
      } 
      else if(day == 7){ // meat - open season
        low = 0;
        high = 25;
      }
      else{ // both - open season
        low = 0;
        high = 68;
      }
  }
  // if we have already recommended all the available foods
  // for this session then we return 
  if(already_recommended_num == high - low + 1) return {"FTASATE TO", "TELOS!", NULL};

  uint8_t rand_num;
  if(low > 0) {
    rand_num = random(array[low-1].val + 1, array[high].val + 1);
  }
  else if(low == 0) {
    rand_num = random(0, array[high].val + 1);
  }
  else {
    rand_num = random(array[low + 69 - 1].val + 1, array[high].val + 1);
  }

  // Binary Search Algortihm to find the food item
  int mid = low + (high - low) / 2;
  while (low <= high) {
    mid = low + (high - low) / 2;

    if (array[mid].val == rand_num)
      break;
    else if (array[mid].val < rand_num)
      low = mid + 1;
    else
      high = mid - 1;
  }

  // for array loop around
  if(mid < 0) mid += 69;

  // if this food item has already been recommended before we
  // return NULL and call the the function again, else we return
  // the name of the food.
  if (already_recommended.has(mid)) return {NULL, NULL, NULL};

  // Every time a food is picked to be displayed on screen 
  // it gets added to the set as to not get recommended again this session 
  already_recommended.add(mid);
  ++already_recommended_num;
  return array[mid];
}

// This function takes a string as an arguement and translates it to the appropriate greek (uppercase) characters and prints it on the lcd.
void print_greek (const char* food_item) {

  // mapping the english character string to its equivelant greek character string for printing 
  uint8_t i = 0;
  char c;
  while((c = food_item[i++]) != '\0') {
    
    // Greek Letters A, B, E, Z, H, I, K, M, N, O, P, T, Y, X are present in the english alphabet so they need not be changed
    // The rest need to be either used from the few cases of greek character in the LCDs ROM or the custom characters we made 
    switch(c) {
      case 'G': // GAMMA
        lcd.write((byte)0); // custom
        break;
      case 'D': // DELTA
        lcd.write((byte)1); // custom
        break;
      case '8': // THETA
        lcd.write((byte)2); // custom
        break;
      case 'L': // LAMDA
        lcd.write((byte)3); // custom
        break;
      case 'J': // KSI
        lcd.write((byte)4); // custom
        break;
      case 'P': 
        lcd.write((byte)5); // custom
        break;
      case 'R':
        lcd.write('P');
        break;
      case 'S': // SIGMA
        lcd.write(246); // LCD ROM
        break;
      case 'F': 
        lcd.write((byte)6); // custom
        break;
      case 'Q': // PSI
        lcd.write((byte)7); // custom
        break;
      case 'W': // OMEGA
        lcd.write(244); // LCD ROM
        break;
      default:
        lcd.write(c);
    }

    //SIGMA 246
    //054 psi
    //244 omega
    //Serial.print("yes");
  }

}
