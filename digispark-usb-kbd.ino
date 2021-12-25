
/* This program is for the Digispark (ATTiny85 MCU) USB board.
 * 
 * This acts like a USB keyboard and presses ENTER whenever P0 is pulled to GND.
 */


#include "DigiKeyboard.h"


#define DEBOUNCE_DELAY 2000
#define PIN_LED 1     /* Model A has LED on GPIO1 */
#define PIN_BUTTON 0  /* GPIO0 for button input */
int button = HIGH;
int button_old = HIGH;
int debounce = 0;


void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);   // enable weak pullup

  // Send an initial keystroke to make Digispark enumerate immediately as HID.
  // Otherwise only enumerates after first keystroke.
  DigiKeyboard.sendKeyStroke(0);
}

void loop() {
  // Debounce delay prevents mechanical switch noise from producing double/triple characters.
  debounce++;
  if (debounce < DEBOUNCE_DELAY)
    return;
  debounce = 0;

  // Read the button. Button pulls the pin to GND.
  button = digitalRead(PIN_BUTTON);
  
  if (button == button_old)
    return;
  button_old = button;
  digitalWrite(PIN_LED, button);
  if (button == HIGH)   // button was released
    return;

  // Drops here only first time when button is pressed.
  digitalWrite(PIN_LED, button);
  
  // This is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);
  
  // Println("") will send just an ENTER.
  DigiKeyboard.println("");
}
