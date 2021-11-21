#include <Keypad.h>

const byte ROWS = 1; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '4'}
};

byte rowPins[ROWS] = {D3}; 
byte colPins[COLS] = {D5, D6, D7, D8}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
  delay(5);
}
