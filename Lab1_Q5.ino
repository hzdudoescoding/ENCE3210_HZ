void setup() {
  Serial.begin(9600);
  // 
  int dataArray[] = {4, 1, 2, 3, 7, 1, 2, 3, 5, 1, 2, 3};
  int patternArray[] = {1, 2, 3};
  int patternCount = 0;
  
  // size of arrays, used for detection
  const int dataSize = sizeof(dataArray) / sizeof(dataArray[0]);
  const int patternSize = sizeof(patternArray) / sizeof(patternArray[0]);

  

  // Loop through dataArray, stopping where room available for pattern
  for (int i = 0; i <= dataSize - patternSize; i++) {
    bool match = true;
    // checks for all elements
    for (int j = 0; j < patternSize; j++) {
      if (dataArray[i + j] != patternArray[j]) {
        match = false;
        break;
      }
    }  
    // if statement for pattern matching
    if (match) {
      patternCount++;
    }
  }

  // display results to serial instead of LED
  if (patternCount > 0) {
    Serial.print("Pattern detected! Number of occurrences: ");
    Serial.println(patternCount);
  } else {
    Serial.println("Pattern not detected in the array.");
  }
}

void loop() {
  // N/A
}
