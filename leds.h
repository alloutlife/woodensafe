/* LEDs control */


void _leds_setup( void );

void _green_led_blink_1( void );

void _green_led_blink_3( void );

void _white_led_blink_1( void );

void _white_led_blink_3( void );

void _white_led_on( void );

void _white_led_off( void );


// Combo function: single LED flash + short beep
void _led_with_buzzer_keypress_echo( int nLedNo );
