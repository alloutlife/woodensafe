#include "defines.h"

#define NTD0 -1                    //bass  1#   2#   3#   4#   5#   6#   7# 
#define NTD1 294                   // A    221  248  278  294  330  371  416 
#define NTD2 330                   // B    248  278  294  330  371  416  467
#define NTD3 350                   // C    131  147  165  175  196  221  248
#define NTD4 393                   // D    147  165  175  196  221  248  278
#define NTD5 441                   // E    165  175  196  221  248  278  312
#define NTD6 495                   // F    175  196  221  234  262  294  330
#define NTD7 556                   // G    196  221  234  262  294  330  371
                                   
#define NTDL1 147                  //Alto  1    2    3    4    5    6    7
#define NTDL2 165                  // A    441  495  556  589  661  742  833
#define NTDL3 175                  // B    495  556  624  661  742  833  935
#define NTDL4 196                  // C    262  294  330  350  393  441  495
#define NTDL5 221                  // D    294  330  350  393  441  495  556
#define NTDL6 248                  // E    330  350  393  441  495  556  624
#define NTDL7 278                  // F    350  393  441  495  556  624  661  
                                   // G    393  441  495  556  624  661  742
#define NTDH1 589
#define NTDH2 661                  //high pitch  1#   2#   3#   4#   5#   6#   7#  
#define NTDH3 700                  //    A       882  990  1112 1178 1322 1484 1665   
#define NTDH4 786                  //    B       990  1112 1178 1322 1484 1665 1869 
#define NTDH5 882                  //    C       525  589  661  700  786  882  990
#define NTDH6 990                  //    D       589  661  700  786  882  990  1112
#define NTDH7 1112                 //    E       661  700  786  882  990  1112 1248

void _buzzer_setup( void )
{
  pinMode( BUZZER_PORT, OUTPUT );
}

void _start_play_button_pressed_sound( void )
{
  tone( BUZZER_PORT, NTDH7 );    // Open buzzer
}

void _stop_play_button_pressed_sound( void )
{
  noTone( BUZZER_PORT );         // Turn off the buzzer
}

void _play_confirm_sound( void )
{
  const int nDelayBetween = 60;
  tone( BUZZER_PORT, NTDL1 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL2 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL3 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL4 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL5 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL6 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, NTDL7 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 294 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 330 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 371 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 416 );    // Open buzzer
  delay( nDelayBetween );  
  tone( BUZZER_PORT, 467 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 495 );    // Open buzzer
  delay( nDelayBetween );
  tone( BUZZER_PORT, 556 );    // Open buzzer
  delay( nDelayBetween );
  noTone( BUZZER_PORT );         // Turn off the buzzer
}

void _play_wrong_pin_sound( void )
{
  const int nDelayBetween = 80;
  for( int k = 0; k < 4; ++k ) {
    tone( BUZZER_PORT, NTDL4 );    // Open buzzer
    delay( nDelayBetween );
    tone( BUZZER_PORT, NTDL2 );    // Open buzzer
    delay( nDelayBetween );
  }

  noTone( BUZZER_PORT );         // Turn off the buzzer
}

