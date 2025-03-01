#define bitsize 14

float Vref = 3;
float Vin = .42;
float thresh;
float quantized = 0;
int count;
int bitval;
int bits[bitsize];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Vref /=2;
  thresh = Vref;

  for(count=0; count<bitsize; count++){
    Vref /=2;
    if (Vin >= thresh){
      bitval = 1;
      thresh += Vref;}
    else {
      bitval = 0;
      thresh -= Vref;}
    
    bits[count] = bitval;
    quantized += 2*Vref*bitval;
    Serial.print(quantized);

  while(1);
}
}
