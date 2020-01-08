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

// Attention! Pins 9 and 10 are no more suitable for analogWrite()
Servo _g_servo;    // create servo object to control a servo

void _setup_safe_door( void )
{
  // MOTOL_POWERING_CTRL solves the problems:
  //      1. Current consumption by Servo: ~5ma @ 7.4V
  //      2. Device initialization: being powered the Servo turns the shaft randomly
  //         this leads to the brief opening of a closed safe during the power on procedure (~1 sec)
  pinMode( MOTOL_POWERING_CTRL, OUTPUT );
  digitalWrite( MOTOL_POWERING_CTRL, 0 );
}

void _unlock_safe_door( void )
{
  digitalWrite( MOTOL_POWERING_CTRL, 1 ); // Power on the motor
  _g_servo.attach( MOTOR_CTRL_PORT );    //attachs the servo
  _g_servo.write( 30 );   // to 0 degrees
  delay( 1000 );
  _g_servo.detach();
  digitalWrite( MOTOL_POWERING_CTRL, 0 ); // Power off the motor
}

void _lock_safe_door( void )
{
  digitalWrite( MOTOL_POWERING_CTRL, 1 ); // Power on the motor
  _g_servo.attach( MOTOR_CTRL_PORT );    //attachs the servo
  _g_servo.write( 0 );   // back to 30 degrees
  delay( 1000 );
  _g_servo.detach();
  digitalWrite( MOTOL_POWERING_CTRL, 0 ); // Power off the motor
}
