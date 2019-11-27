#include "defines.h"
#include "buzzer.h"

void _leds_setup( void )
{
  pinMode( LED_GREEN_PORT, OUTPUT );
  pinMode( LED_WHITE_PORT, OUTPUT );
}

void __led_breathe_1( int _port, int delayVal )
{
  for (int a=0; a<=255;a++)  // Loop, PWM control of LED brightness increase
   {
     analogWrite(_port,a);   // PWM output value a (0~255)
     delayMicroseconds(delayVal);                // The duration of the current brightness level. 15ms           
   }
   delay(100);                // 100ms delay
   for (int a=255; a>=0;a--)  // Loop, PWM control of LED brightness Reduced
   {
     analogWrite(_port,a);   // PWM output value a (255~0)
     delayMicroseconds(delayVal);                // The duration of the current brightness level. 15ms 
   }
}

void __led_breathe_3( int _port )
{
  int k = 0;
  for( ; k < 3; ++k ) {
    if( 0 != k ) {
      delay( 100 );
    }

   __led_breathe_1( _port, 20 );
  }
}

void _green_led_blink_1( void ) {
  __led_breathe_1( LED_GREEN_PORT, 300 );
}

void _green_led_blink_3( void ) {
  __led_breathe_3( LED_GREEN_PORT );
}

void _white_led_blink_1( void ) {
  __led_breathe_1( LED_WHITE_PORT, 300 );
}

void _white_led_blink_3( void ) {
  __led_breathe_3( LED_WHITE_PORT );
}

void _test_leds( void )
{
  _green_led_blink_1();
  _white_led_blink_1();
}

void _led_with_buzzer_keypress_echo( int nLedNo )
{
  const int _port = nLedNo;
  const int delayVal = 200;
  
  for (int a=0; a<=255;a++)  // Loop, PWM control of LED brightness increase
  {
    analogWrite(_port,a);   // PWM output value a (0~255)
    delayMicroseconds(delayVal);                //T he duration of the current brightness level. 15ms           
  }

  _start_play_button_pressed_sound();
  
  delay(50);                // 100ms delay

  _stop_play_button_pressed_sound();

  delay(20);
  
  for (int a=255; a>=0;a--)  // Loop, PWM control of LED brightness Reduced
  {
    analogWrite(_port,a);   // PWM output value a (255~0)
    delayMicroseconds(delayVal);                // The duration of the current brightness level. 15ms 
  }
}

void _white_led_on( void )
{
  digitalWrite( LED_WHITE_PORT, 1 );
}

void _white_led_off( void )
{
  digitalWrite( LED_WHITE_PORT, 0 );
}
