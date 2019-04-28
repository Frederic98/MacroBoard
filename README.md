# MacroBoard
This is a small keypad that can trigger multiple keystrokes with a single button press. It is powered by an ATMega32u4, using the Arduino framework.


## Arduino code
The folder MacroBoard contains the Arduino sketch of this project.  
Because the schematic uses I/O pins that are connected to Rx/Tx LEDs on an Arduino Leonardo, a custom board definition has to be used that assigns these LEDs to different pins. Copy the hardware folder into the folder where the Arduino IDE stores all your sketches, and select MacroBoard as the platform.

## EEPROM
The keystrokes are stored in the EEPROM. For each button, there are 4 EEPROM bytes, meaning the board can trigger up to 4 keystrokes for each button. Unused bytes should be set to 0x00. Printable characters use the ASCII value. For the moddifiers and other keys, look at Arduino/libraries/Keyboard/src/Keyboard.h for their hex value.

## Schematic 
https://easyeda.com/Frederic98/macroboard

Top | Bottom
----|-------
![PCB front](https://github.com/Frederic98/MacroBoard/raw/master/MacroBoard_front.png) | ![PCB rear](https://github.com/Frederic98/MacroBoard/raw/master/MacroBoard_rear.png)
