#include <Wire.h>
#include "defines.h"
#include "leds.h"
#include "buzzer.h"
#include "motor.h"
#include "persistents.h"
#include "pinpad.h"
#include "sensors.h"

/*Модули

  Arduino - контроллер
  
  LED зелёного цвета - инцикация успешного выполнения функции (#11)
  LED белого цвета - отладочный LED - вывод текущего режима (#10)
  Buzzer - звуковое сопровождение: (#8)
      - нажатие кнопки
      - правильное действие
      - ошибка
      
 
  ПИН-клавиатура
  ##A0-A5,D6
  
  
  Серво-мотор #9

  Датчик открытия двери сейфа - #7

  Внутри сейфа кнопка PROG - активация режима программирования кода замка (#6)
  

Режимы работы:



0. POWER_UP

  Если дверь закрыта - переход в состояние SAFE SLOSED
  Если дверь открыта - переход в состояние SAFE OPEN

1. CLOSED. Ожидание ввода кода, дверь сейфа закрыта, замок закрыт

  * Начальное значение: 1 1 1 1
  * После ввода цифр нажать A (accept)
  
  * C - clear
  * 5 sec timeout - очистка буфера
  
  Код неправильный - мерцание на красном LED (плохой звук)
  
  Код правильный - мерцание на зеленом LED (хороший звук)
    - Переход в состояние SAFE OPEN
    
  Каким-то образом открылась дверь сейфа (WTF) - Переход в состояние SAFE OPEN

  
2. SAFE OPEN

  - Закрытие двери сейфа, нажатие кнопки C - закрытие сейфа, переход в режим CLOSED
  - Нажата кнопка PROG - переход в состояние PROG MODE

3. PROG MODE

  Зажечь в режиме постоянного свечения белый LED
  
  - Введена последовательность цифр + буква A - запоминаем код, переход в состояние PROG CONFIRM
  - Нажата кнопка PROG - переход в состояние SAFE OPEN
  
4. PROG CONFIRM

  - Введена та же последовательность цифр + буква A - запись нового кода, переход в состояние SAFE OPEN (+ сигнал ACCEPT)
  - Введена другая последовательность цифр + буква A - переход в состояние SAFE OPEN (+ сигнал WRONG)

*/

enum SafeBoxState {
  PowerUp,
  SafeClosed,   // Waiting for the PIN code
  SafeOpen,     // Safe open. Waiting for the door closed or PROG button pressed
  Prog,         // PIN change mode
  ProgConfirm   // PIN change mode (confirm)
};


SafeBoxState g_currentState = PowerUp;


void setup() {
  
  // Setup code
  _leds_setup();
  _setup_safe_door();

  _buzzer_setup();

  _setup_sensors();
  

  // Factory PIN code init
  char* pszCurrentPincode = _read_digits();
  if( !pszCurrentPincode ) {
    _write_digits( "1111", 4 );
  } else {
    delete[] pszCurrentPincode;
  }
}

void loop()
{
  switch( g_currentState ) {

    case PowerUp:
      DoPowerUpProcedure();
      break;
    case SafeClosed:
      DoSafeClosedProcedure();
      break;
    case SafeOpen:
      DoSafeOpenProcedure();
      break;
    case Prog:
      DoProgProcedure( false );
      break;
    case ProgConfirm:
      DoProgProcedure( true );
      break;
  }
}


void DoPowerUpProcedure( void )
{
  if( is_safe_door_open() ) {
    g_currentState = SafeOpen;
  } else {
    g_currentState = SafeClosed;
  }
}

void DoSafeClosedProcedure( void )
{
  // Perform door close action
  _lock_safe_door();
  
  // PIN buffer
  char pszPinBuffer[256];
  int nPinBufferPos = 0;

  for( ; ; ) {

    // wtf? If the reed switch signals door open - emergency leave clode state and proceed to safe open state
    if( is_safe_door_open() ) {
      g_currentState = SafeOpen;
      return;
    }
    
    char ch = _read_pinpad_key_async();

    // Next digit
    if( ( ch >= '0' && ch <= '9' ) || ch == '#' || ch == '*' ) {
      _led_with_buzzer_keypress_echo( LED_WHITE_PORT );
      pszPinBuffer[ nPinBufferPos++ ] = ch;
      if( 256 == nPinBufferPos ) {
        nPinBufferPos = 0;      // Buffer overflow
      }
    }

    else if( ch == 'A' ) {
      
      // Accept button pressed. Perform PIN verification
      bool bPincodeIsCorrect = false;
      
      char* pszCurrentPincode = _read_digits();
      if( !pszCurrentPincode ) {
        // wtf?
        
      } else if( strlen( pszCurrentPincode ) == nPinBufferPos ) {

        if( 0 == memcmp( pszCurrentPincode, pszPinBuffer, nPinBufferPos ) ) {
          bPincodeIsCorrect = true;
        }
        delete[] pszCurrentPincode;
      }

      if( !bPincodeIsCorrect ) {

        // PIN incorrect. Clear PIN buffer
        _play_wrong_pin_sound();
        nPinBufferPos = 0;
      } else {

        // PIN ok, open the safe
        _led_with_buzzer_keypress_echo( LED_GREEN_PORT );
        _play_confirm_sound();
        g_currentState = SafeOpen;
        return;
      }
    }
  }
}

void DoSafeOpenProcedure( void )
{
  _unlock_safe_door();
  
  for( ; ; ) {
    
    // Enter the Closed state  
    if( _read_pinpad_key_async() == 'C' ) {
  
      if( !is_safe_door_open() ) {
        _led_with_buzzer_keypress_echo( LED_GREEN_PORT );
        g_currentState = SafeClosed;
        return;
      } else {
        _led_with_buzzer_keypress_echo( LED_WHITE_PORT );
      }
    }
  
    // Enter the PIN programming mode
    if( is_prog_button_pressed() ) {
      _led_with_buzzer_keypress_echo( LED_WHITE_PORT );
      g_currentState = Prog;
      return;
    }
  }
}

char g_newPinBuffer[32];
char g_newPinBuffer2[32];
int g_newPinPos = 0;
int g_newPinPos2 = 0;


void DoProgProcedure( bool bConfirmMode )
{
  _white_led_on();
  
  char* pPinBuffer = 0;
  int* pPinPos = 0;

  if( !bConfirmMode ) {
    pPinBuffer = g_newPinBuffer;
    pPinPos = &g_newPinPos;
  } else {
    pPinBuffer = g_newPinBuffer2;
    pPinPos = &g_newPinPos2;
  }

  // Starting from the first digit
  *pPinPos = 0;
  
  for( ; ; ) {

    char ch = _read_pinpad_key_async();

    // Next digit
    if( ( ch >= '0' && ch <= '9' ) || ch == '#' || ch == '*' ) {
      _led_with_buzzer_keypress_echo( LED_GREEN_PORT );
      pPinBuffer[ (*pPinPos)++ ] = ch;
      if( 32 == (*pPinPos) ) {
        *pPinPos = 0;      // Buffer overrun - start again
      }
    }

    else if( ch == 'C' || is_prog_button_pressed() ) {

      // Cancel
      _led_with_buzzer_keypress_echo( LED_WHITE_PORT );
      g_currentState = SafeOpen;
      break;
      
    }

    else if( ch == 'A' ) {
      
      // At least one digit has been entered
      if( (*pPinPos) <= 0 ) {
        _play_wrong_pin_sound();
        *pPinPos = 0;

        g_currentState = SafeOpen;
        break;
      }

      // First run - remember the PIN and proceed to PIN confirmation
      if( !bConfirmMode ) {
        g_currentState = ProgConfirm;
        _led_with_buzzer_keypress_echo( LED_GREEN_PORT );
        break;
      }

      // PIN reenter mode. Verify with the first one
      if( g_newPinPos2 != g_newPinPos ) {
        _play_wrong_pin_sound();
        g_currentState = SafeOpen;
        break;
        
      } else  if( 0 != memcmp( g_newPinBuffer, g_newPinBuffer2, g_newPinPos2 ) ) {
        _play_wrong_pin_sound();
        g_currentState = SafeOpen;
        break;
      }
      
      // Flash the new PIN
      _led_with_buzzer_keypress_echo( LED_GREEN_PORT );
      _write_digits( pPinBuffer, *pPinPos );
      g_currentState = SafeOpen;
      _play_confirm_sound();
      break;
    }
  }

  _white_led_off();
}
