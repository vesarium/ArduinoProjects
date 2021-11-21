// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Keypad.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Keypad setup //
const byte ROWS = 1; 
const byte COLS = 4; 
const int codes[][4] = {
  { 2, 3, 4, 8},
  { 2, 6, 2, 4},
  { 9, 4, 8, 6},
  { 1, 5, 8, 1}
};
int livecode[] = { 0, 0, 0, 0};
bool codeStatus[] = { 1, 1, 1, 1}; 
 

char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'}
};

byte rowPins[ROWS] = {D3}; 
byte colPins[COLS] = {D5, D6, D7, D8}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//Keypad setup end

uint32_t redColor = 13107200;
uint32_t whiteColor = 8224125; //50%
uint32_t greenColor = 51200;
uint32_t yellowColor = 16776960;
uint32_t orangeColor = 16753920; 
uint32_t lawngreenColor = 8256000;
uint32_t blueColor = 25855;


int num0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
int num1[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
int num2[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
int num3[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 30, 31, 32, 33, 34};
int num4[] = {0, 1, 2, 3, 4, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 30, 31, 32, 33, 34};
int num5[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 30, 31, 32, 33, 34};
int num6[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
int num7[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
int num8[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
int num9[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 30, 31, 32, 33, 34};
int arrSize = 0;
int incomingByte = 0;
int currentCode = 0;
uint num = 0;

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

void setup() {
  Serial.begin(115200);
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {

  char customKey = customKeypad.getKey();

  
  
  
  
  if (customKey){
    switch (customKey){
      case '0': if (livecode[0]<9){livecode[0]++;}else{livecode[0]=0;}; break;
      case '1': if (livecode[1]<9){livecode[1]++;}else{livecode[1]=0;}; break;
      case '2': if (livecode[2]<9){livecode[2]++;}else{livecode[2]=0;}; break;
      case '3': if (livecode[3]<9){livecode[3]++;}else{livecode[3]=0;}; break;      
    }    
    checkNum();
  }
  delay(5);
  pixels.clear();
  for (int i=0; i<4;i++){
    drawNum(livecode[i],i+1, whiteColor);
  }
  pixels.show();
  delay(50);
  /*

  if (customKey){
    //Serial.println(customKey);
    Serial.print("code:");
    for (int i=0;i<4;i++){ 
      Serial.print(livecode[i]);
    }
    Serial.print("\n\r");
    
  }
  delay(5);
  
  */
  /*
 
  
  
  for(int i=0; i<=50; i++){
    pixels.clear(); // Set all pixel colors to 'off'
    //
    drawNum(random(0, 9),1, whiteColor);   
    drawNum(random(0, 9),2, whiteColor);  
    drawNum(random(0, 9),3, whiteColor);   
    drawNum(random(0, 9),4, whiteColor);    
    pixels.show();
    delay(50);
    
  }
  for(int i=0; i<=5; i++){
    pixels.clear();
    pixels.show();
    delay(500);
    drawNum(8, 1, redColor);
    drawNum(8, 2, redColor);
    drawNum(8, 3, redColor);
    drawNum(8, 4, redColor);
    pixels.show();
    delay(500);    
  }
  delay(3000);

  
  for(int i=0; i<=50; i++){
    pixels.clear(); // Set all pixel colors to 'off'
    //
    drawNum(random(0, 9),1, whiteColor);   
    drawNum(random(0, 9),2, whiteColor);  
    drawNum(random(0, 9),3, whiteColor);   
    drawNum(random(0, 9),4, whiteColor);    
    pixels.show();
    delay(50);
    
  }
  for(int i=0; i<=5; i++){
    pixels.clear();
    pixels.show();
    delay(500);
    drawNum(8, 1, greenColor);
    drawNum(8, 2, greenColor);
    drawNum(8, 3, greenColor);
    drawNum(8, 4, greenColor);
    pixels.show();
    delay(500);    
  }
  delay(3000);


  */
  
  /*
  for(int i=0; i<sizeof(num01)/sizeof(int); i++) { 
    pixels.setPixelColor(num01[i], pixels.gamma32(pixels.Color(0, 125, 0)));
    delay(DELAYVAL); 
    
  }
  pixels.show();
  delay (1000);
  */
  
  
}

void checkNum(){
bool statusFlag = false;
  for (int i = 0; i<4; i++){
    
    if(livecode[i] == codes[currentCode][i] && codeStatus[currentCode] == 1){
      statusFlag = true;
      
    }else{
      statusFlag = false;
      
      break;
    }
  } 

  if(statusFlag == true){
    statusFlag = false;
    
    for(int i=0; i<=3; i++){
      pixels.clear();
      pixels.show();
      delay(500);
      for (int i=0;i<4;i++){
        drawNum(livecode[i], i+1, yellowColor);       
      }      
      pixels.show();
      delay(500);    
    }
    pixels.clear();
    for (int i=0;i<4;i++){
        livecode[i]=0;       
    }
    pixels.show();
    if (currentCode<5){  currentCode++; }else {currentCode=0;}
  }
  /*
  if(pos1 == codes[0][0] && pos2 == codes[0][1] && pos3 == codes[0][2] && pos4 == codes[0][3] && codeStatus[0] == 1){
    
    for(int i=0; i<=3; i++){
      pixels.clear();
      pixels.show();
      delay(500);
      drawNum(pos1, 1, yellowColor);
      drawNum(pos2, 2, orangeColor);
      drawNum(pos3, 3, lawngreenColor);
      drawNum(pos4, 4, greenColor);
      pixels.show();
      delay(500);    
    }
    delay(3000);
    Serial.println("code:" + String(pos1) + String(pos2) + String(pos3) + String(pos4) );
    pos1=0;pos2=0;pos3=0;pos4=0; codeStatus[0]=0; 
  }  
  
  if(pos1 == code2[0] && pos2 == code2[1] && pos3 == code2[2] && pos4 == code2[3]){
    for(int i=0; i<=3; i++){
      pixels.clear();
      pixels.show();
      delay(500);
      drawNum(pos1, 1, yellowColor);
      drawNum(pos2, 2, orangeColor);
      drawNum(pos3, 3, redColor);
      drawNum(pos4, 4, greenColor);
      pixels.show();
      delay(500);    
    }
    delay(3000);
    pos1=0;pos2=0;pos3=0;pos4=0;  
  }  
  
  if(pos1== 8 && pos2 == 2 && pos3 == 4 && pos4 == 6){
    for(int i=0; i<=3; i++){
      pixels.clear();
      pixels.show();
      delay(500);
      drawNum(pos1, 1, redColor);
      drawNum(pos2, 2, orangeColor);
      drawNum(pos3, 3, yellowColor);
      drawNum(pos4, 4, greenColor);
      pixels.show();
      delay(500);    
    }
    delay(3000);
    pos1=0;pos2=0;pos3=0;pos4=0;  
  }  

  */
}

void drawNum(int num, int col, uint32_t numColor){
  int* arrNum;
  uint16_t i, j;
  
  switch (num){
    case 0: arrSize = (sizeof(num0)/sizeof(int)); arrNum = num0; break;
    case 1: arrSize = (sizeof(num1)/sizeof(int)); arrNum = num1; break;
    case 2: arrSize = (sizeof(num2)/sizeof(int)); arrNum = num2; break;
    case 3: arrSize = (sizeof(num3)/sizeof(int)); arrNum = num3; break;
    case 4: arrSize = (sizeof(num4)/sizeof(int)); arrNum = num4; break;
    case 5: arrSize = (sizeof(num5)/sizeof(int)); arrNum = num5; break;
    case 6: arrSize = (sizeof(num6)/sizeof(int)); arrNum = num6; break;
    case 7: arrSize = (sizeof(num7)/sizeof(int)); arrNum = num7; break;
    case 8: arrSize = (sizeof(num8)/sizeof(int)); arrNum = num8; break;
    case 9: arrSize = (sizeof(num9)/sizeof(int)); arrNum = num9; break;   
    
  }  
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.  
  
  
    for(i=0; i<arrSize; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      //pixels.setPixelColor(arrNum[i], pixels.gamma32(pixels.Color(125,0,125)));
      switch (col){
        case 1: pixels.setPixelColor(arrNum[i], pixels.gamma32(numColor)); break;
        case 2: pixels.setPixelColor(arrNum[i] + 35, pixels.gamma32(numColor)); break;
        case 3: pixels.setPixelColor(arrNum[i] + 70, pixels.gamma32(numColor)); break;
        case 4: pixels.setPixelColor(arrNum[i] + 105, pixels.gamma32(numColor)); break;
      }
      
      //delay(DELAYVAL); // Pause before next pass through loop
      
    }  
}

   
void EEPROM_write(String buffer, int N) //Запись в eeprom 
     {
     EEPROM.begin(512); 
     delay(10);
     for (int L = 0; L < 32; ++L) 
         {
         EEPROM.write(N + L, buffer[L]);
         }
     EEPROM.commit();
     }


String EEPROM_read (int min, int max) //Чтение eeprom
     {
     EEPROM.begin(512); 
     delay(10); 
     String buffer;
     for (int L = min; L < max; ++L)
     if (isAlphaNumeric(EEPROM.read(L)))
     buffer += char(EEPROM.read(L));
     return buffer;
     }
