void setup() {
  Serial.begin(9600);

  // example array
  char letters[] = {'R', 'G', 'B', 'a', 'r', 'D', 'g', 'B', 'b', 'r'};
  int numLetters = sizeof(letters) / sizeof(letters[0]);

  // counter per letter
  int rCount = 0;
  int gCount = 0;
  int bCount = 0;

  // Scan the array
  for (int i = 0; i < numLetters; i++) {
    // converting lowercase to uppercase to simplify counting
    char c = toupper(letters[i]);
    
    if (c == 'R') {
      rCount++;
    } 
    else if (c == 'G') {
      gCount++;
    } 
    else if (c == 'B') {
      bCount++;
    }
  }

  // print results to monitor
  Serial.print("Number of R letters: ");
  Serial.println(rCount);

  Serial.print("Number of G letters: ");
  Serial.println(gCount);

  Serial.print("Number of B letters: ");
  Serial.println(bCount);
}

void loop() {
 // N/A
}
