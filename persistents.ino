
#include <EEPROM.h>

void _write_digits( const char* pszString, unsigned int nLen )
{
  // Allow max 255 digits
  nLen &= 0x000000FF;
  
  // 4 bytes signature
  EEPROM.write( 0, 'F' );
  EEPROM.write( 1, 'c' );
  EEPROM.write( 2, 'u' );
  EEPROM.write( 3, 'k' );

  // 1 byte size
  EEPROM.write( 4, (unsigned char)nLen );  

  for( int k = 0; k < nLen; ++k ) {
    EEPROM.write( 5 + k, pszString[ k ] );
  }
}

char* _read_digits( void )
{
  // Signature
  if(
       EEPROM[ 0 ] != 'F' 
    || EEPROM[ 1 ] != 'c' 
    || EEPROM[ 2 ] != 'u' 
    || EEPROM[ 3 ] != 'k' 
  ) {
    return 0;
  }

  // Length
  unsigned char nLen = EEPROM[ 4 ];

  char* pszResult = new char[ nLen + 1 ];

  // Read blocks
  for( int k = 0; k < (int)nLen; ++k ) {
    pszResult[ k ] = EEPROM[ 5 + k ];
  }
  pszResult[ nLen ] = 0;

  return pszResult;
}
