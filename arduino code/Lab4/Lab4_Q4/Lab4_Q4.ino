#include <OneWire.h>
#include <DallasTemperature.h>

// PIN DEFINITIONS
#define ONE_WIRE_BUS   8   // TEMP SENSOR
#define LED_PIN        9   // PWM OUTPUT (FAN/LED)
#define BUTTON_ONOFF   2   // BUTTON FOR ON/OFF
#define BUTTON_SENS    3   // BUTTON2: SENSITIVITY

// GLOBAL VARIABLES
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// TIMER1 CONFIG
static const uint16_t PWM_TOP = 499;
// F ~ 16 MHz / (2 * PRESCALER * (1 + ICR1)) 
// PRESCALER=64 => F ≈ 16e6 / (2*64*(499+1)) ~ 250 Hz.

// DUTY CYCLES FOR 3 SENSITIVITIES (OCR1A VALUES):
//   99% of 499 ≈ 495
//   66% of 499 ≈ 329
//   33% of 499 ≈ 165
uint16_t dutyCycles[3] = {495, 329, 165};

volatile bool fanOn = false;           // TRACKS ON/OFF (SAME AS ISR)
volatile uint8_t sensitivityIndex = 0; // 0=>99%, 1=>66%, 2=>33%

// FLAGS
volatile bool toggleFanFlag        = false;
volatile bool cycleSensitivityFlag = false;


// ISR'S, ONE FOR POWER, ONE FOR THE SENSITIVITY
void isrOnOff() {
  toggleFanFlag = true;
}

void isrSens() {
  cycleSensitivityFlag = true;
}

void setup() {
  Serial.begin(9600);

  // TEMP SENSOR INITIALIZATION
  sensors.begin();

  // PIN CONFIG
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_ONOFF, INPUT_PULLUP);
  pinMode(BUTTON_SENS,  INPUT_PULLUP);

  // INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(BUTTON_ONOFF), isrOnOff, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_SENS),  isrSens,  FALLING);

  // CONFIGURING TIMER1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  ICR1 = PWM_TOP;
  // WGM13=1, WGM12=0, WGM11=1, WGM10=0 => MODE 10 (FROM DATASHEET)
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13);
  TCCR1A |= (1 << COM1A1);
  // PRESCALER = 64 => CS11=1 & CS10=1
  TCCR1B |= (1 << CS11) | (1 << CS10);

  // DEFAULT OFF, DC = 0
  OCR1A = 0;
}
// GETS TEMPERATURES
float readTemperatureC() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}
// AVERAGE TEMPERATURE FUNCTION
float getAveragedTemperature() {
  float sumTemp = 0.0;
  const int SAMPLES = 100;
  for (int i = 0; i < SAMPLES; i++) {
    sumTemp += readTemperatureC();
    delay(50);  // 50ms * 100 SAMPLES = 5 sec
  }
  return (sumTemp / SAMPLES);
}

//FAN UPDATE PER LOOP
void updateFanSpeed() {
  if (fanOn) {
    OCR1A = dutyCycles[sensitivityIndex]; 
  } else {
    OCR1A = 0;
  }
}

void loop() {
  // 1) CHECK FAN TOGGLES
  if (toggleFanFlag) {
    toggleFanFlag = false; // CLEAR FLAG
    fanOn = !fanOn; 
    updateFanSpeed();
    Serial.println(fanOn ? "Fan turned ON" : "Fan turned OFF");
  }

  // 2) CHECK SENSITIVITY TOGGLES
  if (cycleSensitivityFlag) {
    cycleSensitivityFlag = false; // CLEAR FLAG
    sensitivityIndex = (sensitivityIndex + 1) % 3; 
    updateFanSpeed();
    Serial.print("SENSITIVITY LEVEL SWITCHED TO: ");
    Serial.println(sensitivityIndex);
  }

  // 3) REDO TEMPERATURE AVERAGE
  float avgTemp = getAveragedTemperature();
  Serial.print("AVERAGE TEMPERATURE (C): ");
  Serial.println(avgTemp);
}
