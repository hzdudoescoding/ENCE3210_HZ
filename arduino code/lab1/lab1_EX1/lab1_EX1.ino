#define myPORTB (*(volatile uint8_t*) 0x25)
#define myDDRB (*(volatile uint8_t*) 0x24)
#define LED_PIN 13;
#define PORTB5 0x20;
void setup() {
  // put your setup code here, to run once:
  (myDDRB) |= 0x20;
}

void loop() {
  // put your main code here, to run repeatedly:
//digitalWrite(LED_PIN,HIGH);
  myPORTB &= ~PORTB5;
  delay(500);
  myPORTB |= PORTB5;
  delay(500);
}
