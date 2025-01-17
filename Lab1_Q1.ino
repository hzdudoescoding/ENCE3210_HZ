//array creation
long results[512];

// function creation, doubles square of number
long doubleSquare(int n) {
  return 2L * n * n;  // 2L calculates in 32-bit space
}

void setup() {
  Serial.begin(9600);

  // populating the array
  for (int i = 0; i < 512; i++) {
    results[i] = doubleSquare(i);
  }

  // display results to serial monitor
  for (int i = 0; i < 512; i++) {
    Serial.print("i = ");
    Serial.print(i);
    Serial.print(" => 2*(i^2) = ");
    Serial.println(results[i]);
  }
}

void loop() {
 
}
