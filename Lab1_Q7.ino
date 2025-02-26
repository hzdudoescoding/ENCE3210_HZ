//pin definition
const int button1Pin = 1;  // first button selects operation
const int button2Pin = 2;  // second button executes operation

//global gariables
int gOperationMode = 1;     // 1 = ADD, 2 = SUB, 3 = MUL (default to addition)
bool gButton1State  = HIGH; // current reading of button1
bool gLastButton1State = HIGH; 
bool gButton2State  = HIGH; // current reading of button2
bool gButton2LastState = HIGH;

// dummy arrays:
int array1[10] = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 };
int array2[10] = { 10,  9,  8,  7,  6,  5,  4,  3,  2,  1 };
int array3[10]; //blank

void setup() {
  Serial.begin(9600);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  // making array3 blank
  for(int i = 0; i < 10; i++){
    array3[i] = 0;
  }

  Serial.println("=== Simple Array Calculator ===");
  Serial.println("Press the first button (pin 2) to change operation mode.");
  Serial.println("Press the second button (pin 3) to apply the selected operation.");
}

void loop() {
  // reads button states
  gButton1State = digitalRead(button1Pin);
 gButton2State = digitalRead(button2Pin);

  // detect falling edge (HIGH -> LOW) on button1
  if (gLastButton1State == HIGH && gButton1State == LOW) {
    // increment operation mode
    operationMode++;
    if (operationMode > 3) {
      operationMode = 1; // loop
    }

    switch (operationMode) {
      case 1:
        Serial.println("Operation selected: ADDITION (press second button to apply)");
        break;
      case 2:
        Serial.println("Operation selected: SUBTRACTION (press second button to apply)");
        break;
      case 3:
        Serial.println("Operation selected: MULTIPLICATION (press second button to apply)");
        break;
    }
  }

  // detect falling edge (HIGH -> LOW) on button2
  if (gButton2LastState == HIGH && gButton2State == LOW) {
    // applies currently selected operation to all elements
    applyOperation();
  }

  // updates button states
  gLastButton1State = gButton1State;
  gButton2LastState = gButton2State;
}

// helper function to apply chosen operation across all elements
void applyOperation() {
  Serial.print("Applying ");
  switch (operationMode) {
    case 1:
      Serial.println("ADDITION to all elements:");
      break;
    case 2:
      Serial.println("SUBTRACTION to all elements:");
      break;
    case 3:
      Serial.println("MULTIPLICATION to all elements:");
      break;
  }

  // compute results and store in array3
  for (int i = 0; i < 10; i++) {
    switch (operationMode) {
      case 1: // addition
        array3[i] = array1[i] + array2[i];
        break;
      case 2: // subtraction
        array3[i] = array1[i] - array2[i];
        break;
      case 3: // multiplication
        array3[i] = array1[i] * array2[i];
        break;
    }
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(array1[i]);
    switch (operationMode) {
      case 1: Serial.print(" + "); break;
      case 2: Serial.print(" - "); break;
      case 3: Serial.print(" * "); break;
    }
    Serial.print(array2[i]);
    Serial.print(" = ");
    Serial.println(array3[i]);
  }

  // instead of LEDs, play message upon completion
  Serial.println("All results have been saved. (Simulating all LEDs ON.)");
  Serial.println("-----------------------------------------------------");
}
