#include "defines.h"


#include <Servo.h>
Servo _g_servo;    // create servo object to control a servo

void _setup_safe_door( void )
{
  _g_servo.attach( MOTOR_CTRL_PORT );    // attaches the servo

  // TODO: detach seems to be handful
}

void _unlock_safe_door( void )
{
  _g_servo.write( 0 );   // to 0 degrees
}

void _lock_safe_door( void )
{
  _g_servo.write( 30 );   // back to 30 degrees
}
