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

// Variable Declaration for button
word button_states = 0;
word button_input;
const word push_mask = 0b0000000000000011;  // to keep the last 2 bits / button inputs
byte disable_button_push = 0;

uint8_t already_recommended_num = 0;


// Food Recommender
typedef struct food_item {
  const char* food_name;
  uint8_t val;
} food_item;

food_item array[69] = {
// Meat - Summer
  { "Koloku8akiaGemista", 2 },

// Meat - FFA 
  { "Pastitsio", 3 },
  { "MakaroniaMeKima", 6 },         // 1 + 3
  { "Kanelonia", 7 },               // 4 + 1
  { "KreasMeRuzi", 10 },             // 5 + 3
  { "Giouvetsi", 12 },              // 8 + 2
  { "KreasMeXulopites", 13 },       // 10 + 1
  { "KreasMeMakaronia", 14 },       // 11 + 1
  { "KreasMePatates", 18 },         // 12 + 4
  { "KreasLemonato", 21 },          // 16 + 3
  { "KreasKokkinisto", 24 },        // 19 + 3
  { "KreasMePoure", 26 },           // 22 + 2
  { "Sukwti", 28 },                 // 24 + 2
  { "Mpiftekia", 31 },              // 26 + 3
  { "Keftedakia", 32 },             // 29 + 1
  { "Spetsofai", 33 },              // 30 + 1
  { "Soutzoukakia", 34 },           // 31 + 1
  { "Mprizoles", 35 },              // 32 + 1
  { "Rolo", 36 },                   // 33 + 1
  { "Papoutsakia", 37 },            // 34 + 1
  { "Thgania", 38 },                // 35 + 1
  
// Meat - Winter
  { "Stifado", 39 },               
  { "Laxanontolmades", 41 },       
  { "Frikase", 42 },               
  { "Mplougourosoupa", 44 },       
  { "Kotosoupa", 45 }, 


// Non Meat - Winter
  { "Qarosoupa", 47 },             // 108 + 2
  { "Xortosoupa", 51 },            // 110 + 4
  { "Fasolada", 53 },              // 114 + 2
  { "Revu8ia", 55 },               // 116 + 2
  { "Fava", 56 },                  // 118 + 1
  { "Traxanas", 57 },              // 119 + 1
  { "Laxanoruzo", 58 },            // 120 + 1
  { "XulopitesMeMelitzanes", 59 },

// Non Meat - FFA
  { "Manestra", 60 },               // 36 + 1
  { "Mpamies", 61 },                // 37 + 1
  { "Spanakoruzo", 63 },            // 38 + 2
  { "Soufle", 65 },                 // 40 + 2
  { "Koloku8okeftedes", 66 },       // 42 + 1
  { "MakaroniaMeSaltsa", 71 },      // 43 + 5
  { "MakaroniaMeManitaria", 72 },   // 48 + 1
  { "Tortelinia", 73 },             // 49 + 1
  { "XulopitesMakries", 74 },       // 50 + 1
  { "Kagianas", 75 },               // 51 + 1
  { "Patatokroketes", 76 },         // 52 + 1
  { "QariPlaki", 77 },              // 53 + 1
  { "XylopitesKontes", 78 },        // 54 + 1
  { "Fakes", 80 },                  // 55 + 2
  { "ManitariaLadoriganh", 81 },    // 57 + 1
  { "ManitariaAlaKrem", 82 },       // 58 + 1
  { "Tsipoures", 86 },              // 59 + 4
  { "Kokkinoqaro", 90 },            // 63 + 4
  { "Mpakaliaros", 93 },            // 67 + 3
  { "Galaios", 94 },                // 70 + 1
  { "Prosfugakia", 95 },            // 71 + 1
  { "Gavros", 97 },                 // 72 + 2
  { "QarakiaThganita", 99 },        // 74 + 2
  { "Garides", 100 },                // 76 + 1
  { "Garidomakaronada", 101 },       // 77 + 1
  { "Jespuria", 102 },               // 78 + 1
  { "Gigantes", 103 },               // 79 + 1
  { "Mpriam", 108 },                 // 80 + 5
  { "PatatesMploum", 111 },          // 85 + 3
  { "Arakas", 114 },                 // 88 + 3
  { "ImamMpailnti", 115 },           
  { "PilafiMeManitaria", 116 },

// Non Meat - Summer
  { "Gemista", 119 },                
  { "Koloku8okorfades", 120 },       
  { "Fasolakia", 122 }
};

// A set that contains all the food items that have been recommended
// in this session, as to not have repeats.
Set already_recommended;



void setup() {
  Serial.begin(9600);
  pinMode(12, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  randomSeed(datetime_seed());

  //rtc
  init_rtc("__DATE__", __TIME__);
}


void loop() {
  button_input = digitalRead(12);
  button_states = button_states << 1;
  button_states = button_states | button_input;

  // small delay between button positon checks to give apropriate time to the end-user
  // and not misinterpret button hold and button push s
  delay(40);

  if ((button_states & push_mask) == 2 && !disable_button_push) {  // button push - Action 1
    digitalWrite(LED_BUILTIN, 1);
    
    char* food = NULL;
    while(food == NULL) {
      food = next_item();
    }
    Serial.println(food);
  } 
  else if (button_states == 0b1111111111111111) {  // button hold - Action 2
    digitalWrite(LED_BUILTIN, 1);
    delay(50);
    digitalWrite(LED_BUILTIN, 0);
    disable_button_push = 1;  // makes sure that
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


const char* next_item() {

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
  if(already_recommended_num == high - low + 1) return "No options left.";

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
  if (already_recommended.has(mid)) return NULL;

  // Every time a food is picked to be displayed on screen 
  // it gets added to the set as to not get recommended again this session 
  already_recommended.add(mid);
  ++already_recommended_num;
  return array[mid].food_name;
}
