// necessary included library
#include <math.h>

// variable creation
static const int NUM_ANGLES = 361;   // cos value storage
float cosRadians[NUM_ANGLES];
float cosDegrees[NUM_ANGLES];        

void setup() {

  //for loop to convert to cosine in radians and degrees
  for (int i = 0; i < NUM_ANGLES; i++) {
    // convert i degrees -> i * (PI/180) radians
    float angleInRadians = i * (PI / 180.0);
    cosRadians[i] = cos(angleInRadians);
    cosDegrees[i] = angleInRadians * (180.0 / PI); 
  }
}

void loop() {}