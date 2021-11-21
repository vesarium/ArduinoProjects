#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// constants won't change. They're used here to set pin numbers:
const int upBtn = 11;     // the number of the pushbutton pin
const int downBtn = 10;     // the number of the pushbutton pin
const int okBtn = 9;     // the number of the pushbutton pin

// defines motors pins numbers
const int motor1stepPin = 5; 
const int motor1dirPin = 4; 
const int motor2stepPin = 7; 
const int motor2dirPin = 6; 
 
// variables will change:
int upBtnState = 0; // variable for reading the pushbutton status
int downBtnState = 0; // variable for reading the pushbutton status
int okBtnState = 0; // variable for reading the pushbutton status
int menuState = 0;
int pause = 1000;
void setup()
{
  pinMode(motor1stepPin,OUTPUT); 
  pinMode(motor1dirPin,OUTPUT);
  pinMode(motor2stepPin,OUTPUT); 
  pinMode(motor2dirPin,OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(okBtn, INPUT_PULLUP);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  printMenu();
}


void loop()
{
  upBtnState = digitalRead(upBtn);
  downBtnState = digitalRead(downBtn);
  okBtnState = digitalRead(okBtn);

  // check if the pushbutton is pressed. If it is, the btnState is HIGH:
  if (upBtnState == LOW) {
    // turn LED on:
    menuState = 0;
    printMenu();
  } else {
    // turn LED off:
    
  } 

  // check if the pushbutton is pressed. If it is, the btnState is HIGH:
  if (downBtnState == LOW) {
    // turn LED on:
    menuState = 1;
    printMenu();
  } else {
    // turn LED off:
    
  }

  if (okBtnState == LOW) {
    // turn LED on:
    moveMenu();
  } else {
    // turn LED off:
    
  }
  
}

void printMenu()
{  
  if (menuState == 0){
    lcd.clear();
    lcd.home();
    lcd.setCursor(6,0);
    lcd.print("MOBEX OU ");
    lcd.setCursor(0,2);
    lcd.print("> Start");  
    lcd.setCursor(0,3);
    lcd.print("  Settings");
  }
  if (menuState == 1){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("MOBEX OU ");
    lcd.setCursor(0,2);
    lcd.print("  Start");  
    lcd.setCursor(0,3);
    lcd.print("> Settings");
  
  }  
}

void moveMenu()
{
  if(menuState==0)
  {  
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("Krutilka V 1.0 ");
    lcd.setCursor(3,2);
    lcd.print("Run Program?");  
    lcd.setCursor(5,3);
    lcd.print("Yes");
    menuState==3;
  }
  moveMotor2toStart();
  moveMotor1();
  moveMotor2();
  moveMotor1();
  moveMotor2();
  moveMotor1();
  
}

void moveMotor1()
{
  digitalWrite(motor1dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay
  for(int x = 0; x < 100; x++) {   // total 800
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(pause); // One second delay

  for(int x = 0; x < 2400; x++) {   // total 800
    digitalWrite(motor1stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor1stepPin,LOW); 
    delayMicroseconds(500); 
  }
  

  /*
  digitalWrite(motor1dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 6400; x++) {
    digitalWrite(motor1stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(motor1stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
  */
  
}

void moveMotor2toStart(){
  
  digitalWrite(motor2dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 1600; x++) {
    digitalWrite(motor2stepPin,HIGH);
    delayMicroseconds(100);
    digitalWrite(motor2stepPin,LOW);
    delayMicroseconds(100);
  }
  delay(1000);
}

void moveMotor2()
{
  digitalWrite(motor2dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 1600; x++) {
    digitalWrite(motor2stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(motor2stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
}
