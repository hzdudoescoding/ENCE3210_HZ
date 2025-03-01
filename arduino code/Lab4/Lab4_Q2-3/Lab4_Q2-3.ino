3#include <OneWire.h>
#include <DallasTemperature.h>

// PINS
#define ONE_WIRE_BUS 8   // TEMP SENSOR (in place of solar panel)
#define SWITCH_PIN   9   // LED (in place of switch)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// BATTERY VARIABLE
int batteryCounter = 50;  

// THRESHOLD (>80 DEGREES INSTEAD OF >2.5V)
const float SOLAR_THRESHOLD = 80.0;

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_PIN, OUTPUT);

  // SENSOR INITIALIZATION
  sensors.begin();
}

void loop() {
  // READ TEMP (taking solar panel input)
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  //IF SUN IS OUT (TEMPC>SOLAR) CHARGE BATTERY FIRST BEFORE SWITCHING
  if (tempC > SOLAR_THRESHOLD) {
    if (batteryCounter < 100) {
      batteryCounter++;
      if (batteryCounter > 100) batteryCounter = 100;
    }
    // DISABLE SWITCH, NOT USING BATTERY POWER
    digitalWrite(SWITCH_PIN, LOW);

  } else {
    // IF SOLAR NOT PRESENT, USE BATTERY POWER
    if (batteryCounter > 0) {
      batteryCounter--;
      if (batteryCounter < 0) batteryCounter = 0;
      // TURN SWITCH ON WHEN USING BATTERY
      digitalWrite(SWITCH_PIN, HIGH);
    } else {
      // BATTERY EMPTY, CANNOT POWER SYSTEM
      digitalWrite(SWITCH_PIN, LOW);
    }
  }

  // PRINT READINGS FOR PROOF OF CONCEPT
  Serial.print("TEMPERATURE MEASURED(solar panel reading)) = ");
  Serial.print(tempC);
  Serial.print(" C,  BATTERY = ");
  Serial.print(batteryCounter);
  Serial.println("%");

  delay(1000); 
}
