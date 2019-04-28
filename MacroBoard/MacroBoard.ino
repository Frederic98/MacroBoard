#include <EEPROM.h>
#include <Keyboard.h>

#define ROW_DDR DDRF
#define ROW_PORT PORTF
#define ROW_PIN PINF
#define ROW_START 4
#define ROW_COUNT 4

#define COL_DDR DDRD
#define COL_PORT PORTD
#define COL_PIN PIND
#define COL_START 3
#define COL_COUNT 5

#define BUTTON_COUNT (ROW_COUNT * COL_COUNT)

#define LED_R 13
#define LED_G 5
#define LED_B 10

struct button_t {
  bool state;
  unsigned long changetime;
  uint8_t keys[4];
};

uint8_t buttonState[COL_COUNT];
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

uint8_t keys[BUTTON_COUNT][4];
button_t buttons[BUTTON_COUNT];

inline byte xy_to_index(byte col, byte row){
  return col * ROW_COUNT + row;
}

void button_press(byte col, byte row) {
  byte index = col * ROW_COUNT + row;
  for (byte i = 0; i < 4; i++) {
    if (buttons[index].keys[i] == 0) break;
    Keyboard.press(buttons[index].keys[i]);
  }
  buttons[index].changetime = millis();
  buttons[index].state = true;
}

void button_release(byte col, byte row) {
  byte index = col * ROW_COUNT + row;
  for (byte i = 0; i < 4; i++) {
    if (buttons[index].keys[i] == 0) break;
    Keyboard.release(buttons[index].keys[i]);
  }
  buttons[index].changetime = millis();
  buttons[index].state = false;
}

void setup() {
  Keyboard.begin();
  for (int i = 0; i < BUTTON_COUNT; i++) {
    buttons[i].state = false;
    buttons[i].changetime = 0;
    EEPROM.get(i * sizeof(buttons[i].keys), buttons[i].keys);
  }

  for (int i = 0; i < ROW_COUNT; i++) {
    ROW_DDR &= ~(1 << (i + ROW_START));   // Set input
    ROW_PORT |= (1 << (i + ROW_START));   // Enable pull-up
  }
  for (int i = 0; i < COL_COUNT; i++) {
    COL_DDR |= (1 << (i + COL_START));    // Set output
    COL_DDR |= (1 << (i + COL_START));    // Set HIGH
    buttonState[i] = 0;
  }
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

void loop() {
  for (uint8_t col = 0; col < COL_COUNT; col++) {                     // Loop over columns
    COL_PORT &= ~(1 << (col + COL_START));                               // Pull column LOW
    delay(1);
    uint8_t state = ROW_PIN >> ROW_START;                               // Read current states of the rows

    if (state != buttonState[col]) {                                    // If the row states changed for this column
      for (uint8_t row = 0; row < ROW_COUNT; row++) {                     // Loop over the rows
        uint8_t rstate = (state >> row) & 1;                                // Get state of this row
        if (rstate != ((buttonState[col] >> row) & 1)) {                    // If state is not same as saed state
          if (!rstate) button_press(col, row);                                // If state is LOW: press key
          else button_release(col, row);                                      // Else: release key
        }                                                                   //
      }                                                                   //
      buttonState[col] = state;                                           // Save new row states for this column
    }                                                                     //
    COL_PORT |= (1 << (col + COL_START));                                // Make column HIGH again
  }
}
