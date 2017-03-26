/*

Gizwits

Built-in I/O wiring:

  • Push Button on GPIO04 
  • Red LED on GPIO15
  • Green LED on GPIO12
  • Blue LED on GPIO13
Photo Resistor on ADC/TOUT/A0 pin

*/
#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include "tools.h"
#include "tools_wifiman.h"
#include "ota_tool.h"
#include <Ticker.h>
#include "mqtt_tool.h"

Ticker ticker;

void reset() {
  //reset settings to defaults
  /*
    WMSettings defaults;
    settings = defaults;
    EEPROM.begin(512);
    EEPROM.put(0, settings);
    EEPROM.end();
  */
  //reset wifi credentials
  WiFi.disconnect();
  delay(1000);
  ESP.reset();
  delay(1000);
}

void setup() {
#ifdef serdebug
  Serial.begin(115200);
#endif

  // initialize the pushbutton pin as an input:
  pinMode(SONOFF_BUTTON, INPUT);

  //set relay pin as output
  pinMode(SONOFF_RELAY, OUTPUT);

  //set led pin as output
  pinMode(SONOFF_LED, OUTPUT);

  turnOff();

  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  wifi_init("ESPMQTTSW");

  init_ota("ESPMQTTSW");

  attachInterrupt(SONOFF_BUTTON, toggleState, CHANGE);

  //entered config mode, make led toggle faster
  // ticker.attach(0.2, tick);
  ticker.detach( );
  digitalWrite(SONOFF_LED, LEDStateON);
  turnOff();

  init_mqtt();
}

void loop() {

  check_ota();

  check_mqtt();

  switch (cmd) {
    case CMD_WAIT:
      break;
    case CMD_BUTTON_CHANGE:
      int currentState = digitalRead(SONOFF_BUTTON);
      if (currentState != buttonState) {
        if (buttonState == butStateON && currentState == butStateOFF) {
          long duration = millis() - startPress;
          if (duration < 10) {
            DebugPrintln("too short press - no action");
          } else if (duration < 5000) {
            DebugPrintln("short press - toggle relay");
            toggle();
            pub_mqtt_toggle();
          } else if (duration < 10000) {
            DebugPrintln("medium press - reset");
            restart();
          } else if (duration < 60000) {
            DebugPrintln("long press - reset settings");
            reset();
          }
        } else if (buttonState == butStateOFF && currentState == butStateON) {
          startPress = millis();
        }
        buttonState = currentState;
      }
      break;
  }

}

