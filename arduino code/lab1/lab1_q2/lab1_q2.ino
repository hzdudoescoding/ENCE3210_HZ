
//initial setup/array creation
#include math.h
int arraysize = 361;
float numbers[arraysize];
float angles[arraysize];
void setup() {
  // put your setup code here, to run once:
//filling numbers array with initial variables
for (int i=0;i<arraysize,i++){
  numbers[i] = i;
}
//filling angles array with cosines
for (int j=0;i<arraysize,i++)
  angles[j] = cos(j);
}

void loop() {
  // put your main code here, to run repeatedly:

}
