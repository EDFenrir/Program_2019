const int buttonPin = 4;    // the number of the pushbutton pin
const int pwmPin = 7;      // the number of the LED pin
int pwmState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

#include "ACS712.h"
#define OVERCURRENT 0.2
/*
  This example shows how to measure DC current
*/

// We have 30 amps version sensor connected to A1 pin of arduino
// Replace with your version if necessary
ACS712 sensor(ACS712_30A, A1);

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(pwmPin, OUTPUT);

  // set initial LED state
  digitalWrite(pwmPin, HIGH);

  // This method calibrates zero point of sensor,
  // It is not necessary, but may positively affect the accuracy
  // Ensure that no current flows through the sensor at this moment
  sensor.calibrate();
}

void loop() {
    // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }
  }
  lastButtonState = reading;
  Serial.println(buttonState);
  
  if (buttonState == HIGH){
      // set the LED:
      digitalWrite(pwmPin, pwmState);
    
      // save the reading. Next time through the loop, it'll be the lastButtonState:
      lastButtonState = reading;
    
      
      // Get current from sensor
      float I = sensor.getCurrentAC();
      
      // Send it to serial
      Serial.println(String("I = ") + I + " A");
      
      // Wait one second before the new cycle
      delay(100);
      if (I >= OVERCURRENT){
        pwmState=HIGH;
        Serial.println("ALTO");  
      }else{
        pwmState=LOW;
        Serial.println("BAIXO");
      }
  }else
        pwmState = HIGH;
  
  digitalWrite(pwmPin,pwmState);
}
