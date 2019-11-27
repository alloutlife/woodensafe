
void _setup_snsors( void ) 
{
  pinMode( SAFE_OPEN_PORT, INPUT_PULLUP );
  pinMode( PROG_BUTTON_PORT, INPUT_PULLUP );
}


bool is_safe_door_open( void )
{
  return LOW == digitalRead( SAFE_OPEN_PORT );
}

bool is_prog_button_pressed( void )
{
  return LOW == digitalRead( PROG_BUTTON_PORT );
}

