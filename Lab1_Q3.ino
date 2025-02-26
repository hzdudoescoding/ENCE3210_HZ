//stores second largest value into global variable

#include <EEPROM.h> // include EEPROM library 
#include <avr/pgmspace.h> // include PROGMEM library
#include <limits.h> //limits for PROGMEM search
#define ARRAY 50 // array size

// array stored in PROGMEM
const int numbers[ARRAY] PROGMEM = {
  12, 34, 56, 78, 90, 11, 45, 67, 89, 23,
  5, 67, 89, 100, 33, 22, 11, 66, 77, 88,
  44, 55, 10, 9, 8, 7, 6, 3, 2, 1,
  21, 31, 41, 51, 61, 71, 81, 91, 13, 25,
  37, 49, 60, 72, 84, 96, 100, 99, 98, 97
};

void setup() {
  Serial.begin(9600); 

  // find second max
  int secondMax = findSecondMax(ARRAY);
  Serial.print("Second Maximum: ");
  Serial.println(secondMax);

  // after completion, save second max to variable
  saveToMemory(ARRAY * sizeof(int), secondMax);
}

void loop() {
  // N/A
}

// find second max in PROGMEM
int findSecondMax(int size) {
  int max = INT_MIN;       
  int secondMax = INT_MIN; // initialize max & second max to smallest possible integer

  for (int i = 0; i < size; i++) {
    int value = pgm_read_word(&numbers[i]); // PROGMEM reading
    if (value > max) {
      secondMax = max; // rewrites second max
      max = value;     // old second becomes new max
    } else if (value > secondMax && value != max) {
      secondMax = value; // rewrites second max
    }
  }

  return secondMax;
}

// memory-writing function
void saveToMemory(int address, int value) {
  // limit-checks EEPROM
  if (address + sizeof(value) <= EEPROM.length()) {
    for (int i = 0; i < sizeof(value); i++) {
      EEPROM.write(address + i, (value >> (i * 8)) & 0xFF); // write byte-by-byte
    }
    Serial.println("Second maximum stored sucessfully.");
  } else {
    Serial.println("Error: Memory exceeds EEPROM limitations.");
  }
}