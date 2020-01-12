#include "defines.h"
#include "buzzer.h"

void _leds_setup( void )
{
  pinMode( LED_GREEN_PORT, OUTPUT );
  pinMode( LED_WHITE_PORT, OUTPUT );
}

void __led_breathe_1( int _port, int delayVal, bool bOn, bool bOff )
{
  if( bOn ) {
    int a=0;
    for (; a<=255;a++)  // Loop, PWM control of LED brightness increase
    {
      analogWrite(_port,a);   // PWM output value a (0~255)
      delayMicroseconds(delayVal);                // The duration of the current brightness level. 15ms           
    }
  }

  if( bOn && bOff ) {
    delay(100);                // 100ms delay
  }

  if( bOff ) {
    int a=255;
    for (; a>=0;a--)  // Loop, PWM control of LED brightness Reduced
    {
      analogWrite(_port,a);   // PWM output value a (255~0)
      delayMicroseconds(delayVal);                // The duration of the current brightness level. 15ms 
    }
  }
}

void _green_led_blink_1( void ) {
  __led_breathe_1( LED_GREEN_PORT, 300, true, true );
}

void _white_led_blink_1( void ) {
  __led_breathe_1( LED_WHITE_PORT, 300, true, true );
}

void _led_with_buzzer_keypress_echo( int nLedNo )
{
  const int _port = nLedNo;
  int delayVal = 300;
  if( nLedNo == LED_WHITE_PORT ) {
    delayVal = 150;
  }
  
  for (int a=0; a<=255;a++)  // Loop, PWM control of LED brightness increase
  {
    analogWrite(_port,a);   // PWM output value a (0 .. 200)
    delayMicroseconds(delayVal);                // The duration of the current brightness level
  }

  _start_play_button_pressed_sound();
  
  delay(20);                // 100ms delay

  _stop_play_button_pressed_sound();

  delay(20);
  
  for (int a=255; a>=0;a--)  // Loop, PWM control of LED brightness Reduced
  {
    analogWrite(_port,a);   // PWM output value a (200 .. 0)
    delayMicroseconds(delayVal);                // The duration of the current brightness level
  }
}

void _white_led_on( void )
{
  __led_breathe_1( LED_WHITE_PORT, 300, true, false );
}

void _white_led_off( void )
{
  __led_breathe_1( LED_WHITE_PORT, 300, false, true );
}
