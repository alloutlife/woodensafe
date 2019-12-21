#include <Keypad.h>
 
const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns

// define the symbols on the buttons of the keypads
// @note: there can be optionaly 'C' button which is used to manually close safe door
// or exit programming mode
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','A'}
};
byte rowPins[ ROWS ] = { A0, A1, A2, A3 }; // connect to the row pinouts of the keypad
byte colPins[ COLS ] = { A4, A5, 6 }; // connect to the column pinouts of the keypad
 
// Initialize an instance of the class NewKeypad
Keypad customKeypad = Keypad( makeKeymap( hexaKeys ), rowPins, colPins, ROWS, COLS ); 

char _read_pinpad_key_async( void )
{
  return customKeypad.getKey();
}
