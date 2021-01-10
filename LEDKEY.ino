#define CLOCK_TYPE CLOCK_INVERT
#define CLOCK_DELAY_US 1

const int strobe_pin =  D5;
const int clock_pin  = D6;
const int data_pin   = D7;

void initLEDKEY(){
  pinMode(strobe_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  sendCommand(0x8f);  // activate
  reset();
}

void sendCommand(uint8_t value)
{
  digitalWrite(strobe_pin, LOW);
  shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, value);
  digitalWrite(strobe_pin, HIGH);
}

void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe_pin, LOW);
  shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, 0x00);
  }
  digitalWrite(strobe_pin, HIGH);
}

void segment7clock() {
  // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment; (contributed by viewwer, not tested by me)
  //dateTime = NTPch.getNTPtime(1.0, 1);

  // check dateTime.valid before using the returned time
  // Use "setSendInterval" or "setRecvTimeout" if required
  //if(dateTime.valid){
    //

    getTime();
    segment7(0,(int) (actualHour/10));
    segment7(1,(int) (actualHour%10));
    segment7(2,(int) 0xff);
    segment7(3,(int) (actualMinute/10));
    segment7(4,(int) (actualMinute%10));
    segment7(5,(int) 0xff);
    segment7(6,(int) (actualsecond/10));
    segment7(7,(int) (actualsecond%10));
    LightStripOn((int) (actualsecond%10));
  /*
  } else {
    LightStripOn();
  }
  */
}

uint8_t readButtons(void)
{
  
  uint8_t buttons = 0;
  digitalWrite(strobe_pin, LOW);
  shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, 0x42);
  pinMode(data_pin, INPUT);
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftInMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US) << i;
    buttons |= v;
  }
  pinMode(data_pin, OUTPUT);
  digitalWrite(strobe_pin, HIGH);
  return buttons;
}

void segment7(int pos, int number)
{
  int maps[10]  = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };
  sendCommand(0x44);  // set single address
  if (number < 10) {
    digitalWrite(strobe_pin, LOW);
    shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, (0xc0+(pos*2))); // 1st digit
    shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, maps[number]);
    digitalWrite(strobe_pin, HIGH);
  } else {
    digitalWrite(strobe_pin, LOW);
    shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, (0xc0+(pos*2))); // 1st digit
    shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, 0x00);
    digitalWrite(strobe_pin, HIGH);
  }
}

void setLed(uint8_t value, uint8_t position)
{
  pinMode(data_pin, OUTPUT);

  sendCommand(0x44);
  digitalWrite(strobe_pin, LOW);
  shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, 0xC1 + (position << 1));
  shiftOutMod(data_pin, clock_pin, LSBFIRST, CLOCK_TYPE, CLOCK_DELAY_US, value);
  digitalWrite(strobe_pin, HIGH);
}
