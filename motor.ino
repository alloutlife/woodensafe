#include "defines.h"

/* solenoid driven version
 *  This version doesn't work because of 1A 12V current that is not feasible by 9V CRONA battery 

 
void _setup_safe_door( void )
{
  pinMode( SOLENOID_CTRL_PORT, OUTPUT );
}

void _unlock_safe_door( void )
{
  // Активируем соленоид замка
  digitalWrite( SOLENOID_CTRL_PORT, 1 );
  delay( 3000 );
  digitalWrite( SOLENOID_CTRL_PORT, 0 );
}

void _lock_safe_door( void )
{
  // Nothing to do
}
*/



#include <Servo.h>
Servo _g_servo;    // create servo object to control a servo

void _setup_safe_door( void )
{
}

void _unlock_safe_door( void )
{
  _g_servo.attach( MOTOR_CTRL_PORT );    //attachs the servo
  _g_servo.write( 0 );   // to 0 degrees
  delay( 1000 );
  _g_servo.detach();
}

void _lock_safe_door( void )
{
  _g_servo.attach( MOTOR_CTRL_PORT );    //attachs the servo
  _g_servo.write( 30 );   // back to 30 degrees
  delay( 1000 );
  _g_servo.detach();
}
