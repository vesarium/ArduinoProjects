/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */

// defines motors pins numbers
const int motor1stepPin = 5; 
const int motor1dirPin = 4; 
const int motor2stepPin = 6; 
const int motor2dirPin = 7; 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(motor1stepPin,OUTPUT); 
  pinMode(motor1dirPin,OUTPUT);
  pinMode(motor2stepPin,OUTPUT); 
  pinMode(motor2dirPin,OUTPUT);
}
void loop() {
  digitalWrite(motor1dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 3200; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
  
  digitalWrite(motor1dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 6400; x++) {
    digitalWrite(motor1stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(motor1stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
