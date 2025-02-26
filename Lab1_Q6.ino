#define BUTTON 1           
volatile bool isRunning = false;    // toggled in ISR

// time delay
const unsigned long time = 500;  

//ISR for button
void buttonISR() {
  isRunning = !isRunning; 
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);

  // attach interrupt to pin 1
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonISR, FALLING);
}

void loop() {
  // runs if IsRunning is true
  while (isRunning) {
    Serial.println("R");
    delay(time);

    // checks per display per loop
    if (!isRunning) break;
    Serial.println("RG");
    delay(time);
    if (!isRunning) break;
    Serial.println("RGB");
    delay(time);
    if (!isRunning) break;
    Serial.println("GB");
    delay(time);
    if (!isRunning) break;
    Serial.println("B");
    delay(time);
    if (!isRunning) break;
    Serial.println("RB");
    delay(time);

  }
}
