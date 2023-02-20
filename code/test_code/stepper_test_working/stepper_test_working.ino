// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}

void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(3200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(3200); 
  }
  delay(500); // One second delay
  
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(3200); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(3200); 
  }
  delay(500); // One second delay
}