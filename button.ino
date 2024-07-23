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

word button_states = 0;
word button_input;

const word push_mask = 0b0000000000000011; // to keep the last 2 bits / button inputs
byte disable_button_push = 0;

typedef struct food_item{
  const char* food_name;
  uint8_t val;
} food_item;


food_item array[69] = {
  { "Pastitsio", 1 },
  { "MakaroniaMeKima", 4 },  // 1 + 3
  { "Kanelonia", 5 },         // 4 + 1
  { "KreasMeRuzi", 8 },       // 5 + 3
  { "Giouvetsi", 10 },        // 8 + 2
  { "KreasMeXulopites", 11 }, // 10 + 1
  { "KreasMeMakaronia", 12 }, // 11 + 1
  { "KreasMePatates", 16 },   // 12 + 4
  { "KreasLemonato", 19 },    // 16 + 3
  { "KreasKokkinisto", 22 },  // 19 + 3
  { "KreasMePoure", 24 },     // 22 + 2
  { "Sukwti", 26 },           // 24 + 2
  { "Mpiftekia", 29 },        // 26 + 3
  { "Keftedakia", 30 },       // 29 + 1
  { "Spetsofai", 31 },        // 30 + 1
  { "Soutzoukakia", 32 },     // 31 + 1
  { "Mprizoles", 33 },        // 32 + 1
  { "Rolo", 34 },             // 33 + 1
  { "Papoutsakia", 35 },      // 34 + 1
  { "Thgania", 36 },          // 35 + 1
  { "Manestra", 37 },         // 36 + 1
  { "Mpamies", 38 },          // 37 + 1
  { "Spanakoruzo", 40 },      // 38 + 2
  { "Soufle", 42 },           // 40 + 2
  { "Koloku8okeftedes", 43 }, // 42 + 1
  { "MakaroniaMeSaltsa", 48 },// 43 + 5
  { "MakaroniaMeManitaria", 49},// 48 + 1
  { "Tortelinia", 50 },       // 49 + 1
  { "XulopitesMakries", 51 }, // 50 + 1
  { "Kagianas", 52 },         // 51 + 1
  { "Patatokroketes", 53 },   // 52 + 1
  { "QariPlaki", 54 },        // 53 + 1
  { "XylopitesKontes", 55 },  // 54 + 1
  { "Fakes", 57 },            // 55 + 2
  { "ManitariaLadoriganh", 58 }, // 57 + 1
  { "ManitariaAlaKrem", 59 }, // 58 + 1
  { "Tsipoures", 63 },        // 59 + 4
  { "Kokkinoqaro", 67 },      // 63 + 4
  { "Mpakaliaros", 70 },      // 67 + 3
  { "Galaios", 71 },          // 70 + 1
  { "Prosfugakia", 72 },      // 71 + 1
  { "Gavros", 74 },           // 72 + 2
  { "QarakiaThganita", 76 },  // 74 + 2
  { "Garides", 77 },          // 76 + 1
  { "Garidomakaronada", 78 }, // 77 + 1
  { "Jespuria", 79 },         // 78 + 1
  { "Gigantes", 80 },         // 79 + 1
  { "Mpriam", 85 },           // 80 + 5
  { "PatatesMploum", 88 },    // 85 + 3
  { "Arakas", 91 },           // 88 + 3
  { "ImamMpailnti", 92 },     // 91 + 1
  { "PilafiMeManitaria", 93 },// 92 + 1
  { "Gemista", 96 },          // 93 + 3
  { "Koloku8akiaGemista", 98 },// 96 + 2
  { "Koloku8okorfades", 99 }, // 98 + 1
  { "Fasolakia", 101 },       // 99 + 2
  { "Stifado", 102 },         // 101 + 1
  { "Laxanontolmades", 104 }, // 102 + 2
  { "Frikase", 105 },         // 104 + 1
  { "Mplougourosoupa", 107 }, // 105 + 2
  { "Kotosoupa", 108 },       // 107 + 1
  { "Qarosoupa", 110 },       // 108 + 2
  { "Xortosoupa", 114 },      // 110 + 4
  { "Fasolada", 116 },        // 114 + 2
  { "Revu8ia", 118 },         // 116 + 2
  { "Fava", 119 },            // 118 + 1
  { "Traxanas", 120 },        // 119 + 1
  { "Laxanoruzo", 121 },      // 120 + 1
  { "XulopitesMeMelitzanes", 122 } // 121 + 1
};


void setup() {
  Serial.begin(300);
  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print(next_item());
}

void loop() {
    button_input = digitalRead(7);
    button_states = button_states << 1;
    button_states = button_states | button_input;

    // small delay between button positon checks to give apropriate time to the end-user
    // and not misinterpret button hold and button push s
    delay(40);  

    if((button_states & push_mask) == 2 && !disable_button_push) { // button push - Action 1
        digitalWrite(LED_BUILTIN, 1);
        //
    }
    else if(button_states == 0b1111111111111111) { // button hold - Action 2
        digitalWrite(LED_BUILTIN, 1); 
        delay(50);
        digitalWrite(LED_BUILTIN, 0);
        disable_button_push = 1; // makes sure that 
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

// ADD DAYS AND WINTER/SUMMER
const char* next_item() {
  randomSeed(11); // add current time as the seed
  uint8_t rand_num = random(0, 121); 

  // Binary Search Algortihm to find the food item 
  int mid, low = 0, high = 68;
  while (low <= high) {
    mid = low + (high - low) / 2;

    if (array[mid].val == rand_num)
      return array[mid].food_name;
    else if (array[mid].val < rand_num)
      low = mid + 1;
    else
      high = mid - 1;
  }

  return array[mid].food_name;
}
