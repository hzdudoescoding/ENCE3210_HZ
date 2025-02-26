#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "I2Cdev.h"
#include "MPU6050.h"
// PIN DEFINITIONS
#define B_POWER        2      
#define B_LOCK         3      
#define B_LIGHT        5      
#define B_UNITS        4       
#define LED            9     
#define TEMP_ONE_WIRE  8      
// GLOBAL FLAG VARIABLES
volatile bool G_POWERFLAG = false;  // POWER TOGGLE
bool          G_LOCKFLAG  = false;  // DISPLAY LOCK CHECK
bool          G_LIGHTFLAG = false;  // LED TOGGLE
bool          G_UNITFLAG  = false;  // CELSIUS - TRUE, FAHRENHEIT - FALSE
// TEMPERATURE VARIABLES (celsius is used later :) )
float temp_f         = 0.0;
float locked_temp_f  = 0.0;
float locked_temp_c  = 0.0;
// LED DEBOUNCING VARIABLES
bool     lastLightButtonState = HIGH;          // INPUT_PULLUP CORRECTION
unsigned long lastLightDebounceTime = 0;
const unsigned long LIGHT_DEBOUNCE_DELAY = 50;  // DEBOUNCING TIME
// LOCK BUTTON HOLDING LOGIC
bool     lastLockButtonState     = HIGH;       // INPUT_PULLUP CORRECTION
unsigned long lockPressStartTime = 0;
bool     lockToggledThisPress    = false;      // CHECK FOR ACCIDENTAL MULTIPLES OF PRESSES
const unsigned long LOCK_HOLD_TIME = 3000;     // LOCK HOLD VALUE (3 SECONDS)
// ONEWIRE VARIABLES (FOR TEMPERATURE SENSOR)
OneWire oneWire(TEMP_ONE_WIRE);
DallasTemperature sensors(&oneWire);
// MPU6050 SETUP
MPU6050 accelgyro;  // MPU OBJECT
int16_t ax, ay, az; // ACCELERATION DATA
bool orientationRightSideUp = true; // TRUE FOR POSITIVE az, FALSE FOR NEGATIVE
// SETUP CODE
void setup() {
  Serial.begin(9600);
// INITIALIZATION OF TEMPERATURE SENSOR
  sensors.begin();
// INITIALIZING MPU6050
  Wire.begin();
  accelgyro.initialize();  
// PINMDOE SETUPS
  pinMode(B_POWER, INPUT_PULLUP);   // POWER BUTTON
  pinMode(B_LOCK,  INPUT_PULLUP);   // LOCK BUTTON ON TIMER
  pinMode(B_LIGHT, INPUT_PULLUP);   // LED TOGGLE BUTTON
  pinMode(LED,     OUTPUT);         // LED 
  digitalWrite(LED, LOW);
// INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(B_POWER), isr_power, RISING);
}
//LOOP CODE
void loop() {
  // STEP 1 - LOCK CHECK
  checkLockHold();
  // STEP 2 - LIGHT TOGGLE CHECK
  checkLightToggle();

  // STEP 3 - TEMPERATURE SENSOR LOGIC - ONLY APPLICABLE IF POWER ACTIVE
  if (G_POWERFLAG) {
    // TEMPERATURE SENSOR CHECK
    sensors.requestTemperatures();
    temp_f = sensors.getTempFByIndex(0);
    // CONVERSION
    float temp_c = (5.0 / 9.0) * (temp_f - 32.0);
    // IF LOCKED, WRITE LOCK VALUES
    if (G_LOCKFLAG) {
      locked_temp_f = temp_f;
      locked_temp_c = temp_c;
    }
// ORIENTATION CHECK (LOOKING FOR RIGHTSIDEUP OR UPSIDEDOWN)
    readOrientation();
// SERIAL PRINTING LOOP
    SerialCheck(temp_f, temp_c);
  } else {
// IF POWER == 0
    Serial.println("SYSTEM POWER OFF.");
  }
// DELAY FOR ACCURACY/CLARITY
  delay(800);
}
// MPU6050 CHECK
void readOrientation() {
  // RAW ACCELERATION VALUES
  accelgyro.getAcceleration(&ax, &ay, &az);
// ORIENTATION CHECK -> ax>0 RIGHTSIDEUP, ax<0 UPSIDEDOWN
  orientationRightSideUp = (az > 0);
}
// PRINTING LOOP
void SerialCheck(float current_f, float current_c) {
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  // PRINT ORIENTATION
  Serial.print("ORIENTATION: ");
  if (orientationRightSideUp) {
    Serial.println("RIGHT SIDE UP");
  } else {
    Serial.println("UPSIDE DOWN");
  }
  // TEMPERATURE PRINTING
  Serial.print("CURRENT TEMPERATURE: ");
  if (G_UNITFLAG) {
    // CELSIUS
    if (G_LOCKFLAG) {
      Serial.print(locked_temp_c);
      Serial.println(" °C (LOCKED)");
    } else {
      Serial.print(current_c);
      Serial.println(" °C");
    }
  } else {
    // Show Fahrenheit
    if (G_LOCKFLAG) {
      Serial.print(locked_temp_f);
      Serial.println(" °F (LOCKED)");
    } else {
      Serial.print(current_f);
      Serial.println(" °F");
    }
  }
}
//ISR FOR POWER 
void isr_power() {
  if (digitalRead(B_POWER) == HIGH) {
    G_POWERFLAG = !G_POWERFLAG;
  }
}
//LOCK HOLD CHECK
void checkLockHold() {
  bool currentLockButtonState = digitalRead(B_LOCK);
  // CHECK/RECORD LAST LOCK STATE CHANGE (LOW<->HIGH)
  if (currentLockButtonState != lastLockButtonState) {
    lockPressStartTime = millis();
  }
  // CHECKS FOR STABILITY AND CHANGE
  if ((millis() - lockPressStartTime) > 50) {  
    static bool lockButtonStableState = HIGH;
    if (currentLockButtonState != lockButtonStableState) {
      lockButtonStableState = currentLockButtonState;
      // IF CHANGE IS LOW, SET START TIME
      if (lockButtonStableState == LOW) {
        lockPressStartTime   = millis();
        lockToggledThisPress = false; // ACCIDENTAL MULTIPLE PRESSES PROTECTION
      } else {
      }
    }
  }
  // IF LAST READING IS LOW, RE-CHECK 
  if (digitalRead(B_LOCK) == LOW) {
    unsigned long heldTime = millis() - lockPressStartTime;
// SWAP LOCK IF HELD AFTER 3 SECONDS
    if (heldTime >= LOCK_HOLD_TIME && !lockToggledThisPress) {
      G_LOCKFLAG = !G_LOCKFLAG;
      lockToggledThisPress = true;
    }
  }
  // UPDATE BUTTON STATE
  lastLockButtonState = currentLockButtonState;
}
// LIGHT TOGGLE CHECK
void checkLightToggle() {
  bool currentButtonState = digitalRead(B_LIGHT);
// CHECK LAST BUTTON STATE AGAINST CURRENT
  if (currentButtonState != lastLightButtonState) {
    lastLightDebounceTime = millis();
  }
  if ((millis() - lastLightDebounceTime) > LIGHT_DEBOUNCE_DELAY) {
    static bool buttonStableState = HIGH; // STABLE BUTTON STATE
    // SECONDARY CHECK
    if (currentButtonState != buttonStableState) {
      buttonStableState = currentButtonState;
      // PULLUP MEANS HIGH -> PRESS -> LOW INSTEAD OF LOW -> PRESS -> HIGH
      if (buttonStableState == LOW) {
        G_LIGHTFLAG = !G_LIGHTFLAG;
        digitalWrite(LED, G_LIGHTFLAG ? HIGH : LOW);
      }
    }
  }
  lastLightButtonState = currentButtonState;
}