#define SONOFF_BUTTON    4
#define SONOFF_RELAY    12
#define SONOFF_LED      13


#define relStateOFF LOW
#define relStateON HIGH
#define LEDStateOFF HIGH
#define LEDStateON LOW
#define butStateOFF HIGH
#define butStateON LOW

const int CMD_WAIT = 0;
const int CMD_BUTTON_CHANGE = 1;
int cmd = CMD_WAIT;

int relayState = relStateOFF;

//inverted button state
int buttonState = HIGH;

static long startPress = 0;

void tick()
{
  //toggle state
  int state = digitalRead(SONOFF_LED);  // get the current state of GPIO1 pin
  digitalWrite(SONOFF_LED, !state);     // set pin to the opposite state
}

void setState(int s) {

  relayState = s;

  DebugPrintln(relayState);

  digitalWrite(SONOFF_RELAY, relayState);
  if (relayState == relStateOFF) {
//    digitalWrite(SONOFF_LED, LEDStateOFF);
  }
  else {
//    digitalWrite(SONOFF_LED, LEDStateON);
  }

}

void turnOn() {

  setState(relStateON);
}

void turnOff() {

  setState(relStateOFF);
}

void toggleState() {
  cmd = CMD_BUTTON_CHANGE;
}

void toggle() {
  DebugPrintln("toggle state");
  relayState = relayState == relStateOFF ? relStateON : relStateOFF;
  setState(relayState);
}

void restart() {
  ESP.reset();
  delay(1000);
}


