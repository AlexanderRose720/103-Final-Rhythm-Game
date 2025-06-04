#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

#define speakerPin A0

int gameState = 0;

bool switchState;

int gameDifficulty = 1;
int gameVolume = 153;
int gameColor = 9;

int gameScore = 0;
int Correct;

float beatLength;

int leftIndex = 0;
int leftSeedNum = 0;

int rightIndex = 0;
int rightSeedNum = 1;

int accelIndex = 0;
int accelSeedNum = 2;

bool speechFlag = false;

//from:
///////////////////////////////////////////////////////////////////////////////
// Talkie library
// Copyright 2011 Peter Knight
// This code is released under GPLv2 license.
//
// The following phrases are derived from VM61002 ROM
//
// A male voice with a US accent.
//
// These phrases have a very military bias
// with lots of very useful engineering words.
// They also have good expression.
// 
const uint8_t spTHREE[]         PROGMEM = {0x0C,0xE8,0x2E,0x94,0x01,0x4D,0xBA,0x4A,0x40,0x03,0x16,0x68,0x69,0x36,0x1C,0xE9,0xBA,0xB8,0xE5,0x39,0x70,0x72,0x84,0xDB,0x51,0xA4,0xA8,0x4E,0xA3,0xC9,0x77,0xB1,0xCA,0xD6,0x52,0xA8,0x71,0xED,0x2A,0x7B,0x4B,0xA6,0xE0,0x37,0xB7,0x5A,0xDD,0x48,0x8E,0x94,0xF1,0x64,0xCE,0x6D,0x19,0x55,0x91,0xBC,0x6E,0xD7,0xAD,0x1E,0xF5,0xAA,0x77,0x7A,0xC6,0x70,0x22,0xCD,0xC7,0xF9,0x89,0xCF,0xFF,0x03};
const uint8_t spTWO[]           PROGMEM = {0x06,0xB8,0x59,0x34,0x00,0x27,0xD6,0x38,0x60,0x58,0xD3,0x91,0x55,0x2D,0xAA,0x65,0x9D,0x4F,0xD1,0xB8,0x39,0x17,0x67,0xBF,0xC5,0xAE,0x5A,0x1D,0xB5,0x7A,0x06,0xF6,0xA9,0x7D,0x9D,0xD2,0x6C,0x55,0xA5,0x26,0x75,0xC9,0x9B,0xDF,0xFC,0x6E,0x0E,0x63,0x3A,0x34,0x70,0xAF,0x3E,0xFF,0x1F};
const uint8_t spONE[]           PROGMEM = {0x66,0x4E,0xA8,0x7A,0x8D,0xED,0xC4,0xB5,0xCD,0x89,0xD4,0xBC,0xA2,0xDB,0xD1,0x27,0xBE,0x33,0x4C,0xD9,0x4F,0x9B,0x4D,0x57,0x8A,0x76,0xBE,0xF5,0xA9,0xAA,0x2E,0x4F,0xD5,0xCD,0xB7,0xD9,0x43,0x5B,0x87,0x13,0x4C,0x0D,0xA7,0x75,0xAB,0x7B,0x3E,0xE3,0x19,0x6F,0x7F,0xA7,0xA7,0xF9,0xD0,0x30,0x5B,0x1D,0x9E,0x9A,0x34,0x44,0xBC,0xB6,0x7D,0xFE,0x1F};
///////////////////////////////////////////////////////////////////////////////

//from:
///////////////////////////////////////////////////////////////////////////////
// Circuit Playground Shake Detect
// link: (https://learn.adafruit.com/circuit-playground-d6-dice/shake-detect) 
// Author: Carter Nelson
// MIT License (https://opensource.org/licenses/MIT)
const int shakeThreshold = 15;
float X;
float Y;
float Z;
float totalAccel;
//////////////////////////////////////////////////////////////////////////////

AsyncDelay toneTimer;
AsyncDelay beatTimer;

int Colors[10] = {25, 50, 75, 100, 125, 150, 175, 200, 225, 250};

bool switchFlag; // flag to check switch state
bool LBFlag; // flag to check button press
bool RBFlag; // flag to check button press

bool accelFlag = false;

const int switchbounceDuration = 150; // length in Millis to account for hardware bounce
volatile long switchbounceTime = 0;

const int LBbounceDuration = 150; // length in Millis to account for hardware bounce
volatile long LBbounceTime = 0;

const int RBbounceDuration = 150; // length in Millis to account for hardware bounce
volatile long RBbounceTime = 0;

int gameSeed[30] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void Switch() {
  if (abs(millis() - switchbounceTime) > switchbounceDuration) { //checks that it has been 150 milliseconds since last interrupt to prevent hardware bounce
    switchFlag = !switchFlag; // switch flag state
    switchbounceTime = millis();
  }
}

void LB() {
  if (abs(millis() - LBbounceTime) > LBbounceDuration) { //checks that it has been 150 milliseconds since last interrupt to prevent hardware bounce
    LBFlag = true; // switch flag state
    LBbounceTime = millis();
  }
}

void RB() {
  if (abs(millis() - RBbounceTime) > RBbounceDuration) { //checks that it has been 150 milliseconds since last interrupt to prevent hardware bounce
    RBFlag = true; // switch flag state
    RBbounceTime = millis();
  }
}

void setup() {

  CircuitPlayground.begin(); // turn on elements on board
  Serial.begin(9600); // begin serial monitor at 9600 baud

  attachInterrupt(4, LB, RISING);
  attachInterrupt(5, RB, RISING);
  attachInterrupt(7, Switch, CHANGE);

  switchFlag = CircuitPlayground.slideSwitch();
  switchState = switchFlag;

  randomSeed(map(CircuitPlayground.mic.soundPressureLevel(10), 50, 100, 0, 1000000));

  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);

}

void loop() {

  switch(gameState) {
    case 0:
      mainMenu();
      break;
    case 1:
      initialization();
      break;
    case 2:
      settings();
      break;
    case 3:
      game();
      break;
    default:
      break;
  }

}

void mainMenu() {

  for(int i = 1; i < 4; i++) {

    CircuitPlayground.setPixelColor(i, 0, 255, 0);
    CircuitPlayground.setPixelColor(i+5, 255, 255, 255);
  }
  if (gameDifficulty == 1) {
    CircuitPlayground.setPixelColor(4, 0, 255, 0);
    CircuitPlayground.setPixelColor(5, 0, 255, 0);
  }
  else if (gameDifficulty == 2) {
    CircuitPlayground.setPixelColor(4, 255, 255, 0);
    CircuitPlayground.setPixelColor(5, 255, 255, 0);
  }
  else if (gameDifficulty == 3) {
    CircuitPlayground.setPixelColor(4, 255, 0, 0);
    CircuitPlayground.setPixelColor(5, 255, 0, 0);
  }

  if(LBFlag) {
    gameState = 1;
    CircuitPlayground.clearPixels();
    LBFlag = false;
  }
  else if(RBFlag) {
    gameState = 2;
    CircuitPlayground.clearPixels();
    RBFlag = false;
  }
  if(switchFlag != switchState) {
      gameDifficulty += 1;
      if(gameDifficulty > 3) {
        gameDifficulty = 1;
      }
      switchState = switchFlag;
      Serial.println(gameDifficulty);
  }


}

void settings() {
  if(LBFlag) {
    // Volume

    CircuitPlayground.clearPixels();

    if(RBFlag) {
      gameVolume += 51;
      if(gameVolume < 0) {
        gameVolume = 255;
      }
      else if(gameVolume > 255) {
        gameVolume = 153;
      }
      makeSound(440, 250);
      Serial.println(gameVolume);
      RBFlag = false;
    }
    if(toneTimer.isExpired()) {
      noTone(speakerPin);
    }

    if(gameVolume == 153) {
      CircuitPlayground.setPixelColor(4, 255, 255, 255);
      CircuitPlayground.setPixelColor(5, 255, 255, 255);
    }
    else if(gameVolume == 204) {
      CircuitPlayground.setPixelColor(2, 255, 255, 255);
      CircuitPlayground.setPixelColor(7, 255, 255, 255);
    }
    else if(gameVolume == 255) {
      CircuitPlayground.setPixelColor(0, 255, 255, 255);
      CircuitPlayground.setPixelColor(9, 255, 255, 255);
    }

    if(switchFlag != switchState) {
      LBFlag = false;
      switchState = switchFlag;
      CircuitPlayground.clearPixels();
    }

    //
  }
  else if(RBFlag) {
    // Colors


    for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(Colors[i]));
    }

    delay(250);

    CircuitPlayground.setPixelColor(gameColor, 0, 0, 0);

    delay(250);



    if(LBFlag) {
      gameColor += 1;
      LBFlag = false;
    }

    if(gameColor < 0) {
      gameColor = 9;
    }
    else if(gameColor > 9) {
      gameColor = 0;
    }

    if(switchFlag != switchState) {
      RBFlag = false;
      switchState = switchFlag;
      CircuitPlayground.clearPixels();
    }

    //
  }
  else {
    for(int i = 6; i < 9; i++) {
      CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(Colors[gameColor]));
    }
    for(int i = 1; i < 4; i++) {
      CircuitPlayground.setPixelColor(i, 255, 255, 255);
    }
    if(switchFlag != switchState) {
      gameState = 0;
      switchState = switchFlag;
      CircuitPlayground.clearPixels();
    }

  }
  
  
}

void initialization() {
  for(int i = 0; i < 30; i++) {
    gameSeed[i] = random(1, 10);
    Serial.print(gameSeed[i]);
  }
  Serial.println();

  switch(gameDifficulty) {
    case 1:
      beatTimer.start(600, AsyncDelay::MILLIS);
      beatLength = 600;
      break;
    case 2:
      beatTimer.start(468.75, AsyncDelay::MILLIS);
      beatLength = 468.75;
      break;
    case 3:
      beatTimer.start(400, AsyncDelay::MILLIS);
      beatLength = 400;
      break;
    default:
      beatTimer.start(600, AsyncDelay::MILLIS);
      beatLength = 600;
      break;
  }
  leftIndex = 0;
  leftSeedNum = 0;
  rightIndex = 0;
  rightSeedNum = 1;
  accelIndex = 0;
  accelSeedNum = 2;


  gameScore = 0;

  gameState = 3;
}

void game() {

  //from:
  ///////////////////////////////////////////////////////////////////////////////
  // Circuit Playground Shake Detect
  //
  // Author: Carter Nelson
  // MIT License (https://opensource.org/licenses/MIT)
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;
  ///////////////////////////////////////////////////////////////////////////////

  if(beatTimer.isExpired()) {
    if(!speechFlag) {
      makeSound(440, 250);
    }
    if(leftSeedNum <= 28) {
      leftSide(gameSeed[leftSeedNum]);
    }
    if(rightSeedNum <= 29) {
      rightSide(gameSeed[rightSeedNum]);
    }
    if(accelSeedNum <= 30) {
      Accel(gameSeed[accelSeedNum]);
    }
    beatTimer.repeat();
    if(leftSeedNum > 28 && rightSeedNum > 29 && accelSeedNum > 30) {
      noTone(speakerPin);
      gameEnd();
    }
  }


  if(switchFlag != switchState) {
    gameState = 0;
    switchState = switchFlag;
    CircuitPlayground.clearPixels();
    noTone(speakerPin);
  }
  if(toneTimer.isExpired()) {
    noTone(speakerPin);
  }
  if(totalAccel > shakeThreshold) {
    accelFlag = true;
  }

}

void leftSide(int digit) {
  if(leftIndex < digit) {
    CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(Colors[gameColor]));
    leftIndex += 1;
  }
  else if(leftIndex == digit) {
    CircuitPlayground.setPixelColor(0, 0, 0, 0);
    CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(Colors[gameColor]));
    leftIndex += 1;
  }
  else if(leftIndex == (digit+1)) {
    CircuitPlayground.setPixelColor(1, 0, 0, 0);
    CircuitPlayground.setPixelColor(2, CircuitPlayground.colorWheel(Colors[gameColor]));
    leftIndex += 1;
  }
  else if(leftIndex == (digit+2)) {
    CircuitPlayground.setPixelColor(2, 0, 0, 0);
    CircuitPlayground.setPixelColor(3, CircuitPlayground.colorWheel(Colors[gameColor]));
    leftIndex += 1;
  }
  else if(leftIndex == (digit+3)) {
    CircuitPlayground.setPixelColor(3, 0, 0, 0);
    CircuitPlayground.setPixelColor(4, CircuitPlayground.colorWheel(Colors[gameColor]));
    leftIndex += 1;
  }
  else if(leftIndex == (digit+4)) {
    leftIndex = 0;
    CircuitPlayground.setPixelColor(4, 0, 0, 0);
    leftSeedNum += 3;
  }
  if(LBFlag && ((leftIndex ==(digit+4)))) {
    CircuitPlayground.setPixelColor(4, 0, 255, 0);
    Serial.println("Left Score!");
    LBFlag = false;
    gameScore += 1;
  }
  else if(LBFlag) {
    CircuitPlayground.setPixelColor(4, 255, 0, 0);
    LBFlag = false;
  }
  if(leftIndex == 0) {
    if (LBFlag) {
    LBFlag = false;
    }
    else {
      CircuitPlayground.setPixelColor(4, 0, 0, 0);
    }
  }
  
  
}

void rightSide(int digit) {
  if(rightIndex < digit) {
    CircuitPlayground.setPixelColor(9, CircuitPlayground.colorWheel(Colors[gameColor]));
    rightIndex += 1;
  }
  else if(rightIndex == digit) {
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
    CircuitPlayground.setPixelColor(8, CircuitPlayground.colorWheel(Colors[gameColor]));
    rightIndex += 1;
  }
  else if(rightIndex == (digit+1)) {
    CircuitPlayground.setPixelColor(8, 0, 0, 0);
    CircuitPlayground.setPixelColor(7, CircuitPlayground.colorWheel(Colors[gameColor]));
    rightIndex += 1;
  }
  else if(rightIndex == (digit+2)) {
    CircuitPlayground.setPixelColor(7, 0, 0, 0);
    CircuitPlayground.setPixelColor(6, CircuitPlayground.colorWheel(Colors[gameColor]));
    rightIndex += 1;
  }
  else if(rightIndex == (digit+3)) {
    CircuitPlayground.setPixelColor(6, 0, 0, 0);
    CircuitPlayground.setPixelColor(5, CircuitPlayground.colorWheel(Colors[gameColor]));
    rightIndex += 1;
  }
  else if(rightIndex == (digit+4)) {
    rightIndex = 0;
    CircuitPlayground.setPixelColor(5, 0, 0, 0);
    rightSeedNum += 3;
  }
  if(RBFlag && ((rightIndex ==(digit+4)))) {
    CircuitPlayground.setPixelColor(5, 0, 255, 0);
    Serial.println("Right Score!");
    gameScore += 1;
    RBFlag = false;
  }
  else if(RBFlag) {
    CircuitPlayground.setPixelColor(5, 255, 0, 0);
    RBFlag = false;
  }
  if(rightIndex == 0) {
    if (RBFlag) {
    RBFlag = false;
    }
    else {
      CircuitPlayground.setPixelColor(5, 0, 0, 0);
    }
  }
  
  
}

void Accel(int digit) {
  totalAccel = sqrt(X*X + Y*Y + Z*Z);
  if(accelIndex < digit) {
    
    accelIndex += 1;
  }
  else if(accelIndex == digit) {
    
    accelIndex += 1;
  }
  else if(accelIndex == (digit+1)) {
    speechFlag = true;
    CircuitPlayground.speaker.say(spTHREE);
    accelIndex += 1;
  }
  else if(accelIndex == (digit+2)) {
    CircuitPlayground.speaker.say(spTWO);
    accelIndex += 1;
  }
  else if(accelIndex == (digit+3)) {
    CircuitPlayground.speaker.say(spONE);
    speechFlag = false;
    accelIndex += 1;
  }
  else if(accelIndex == (digit+4)) {
    
    accelIndex = 0;
    accelSeedNum += 3;
  }
  if((accelFlag) && ((accelIndex ==(digit+4)))) {
    Serial.println("accel Score!");
    gameScore += 1;
  }
  else if(accelFlag) {
  }
  if(accelIndex == 0) {

  }
  
  
}

void gameEnd() {
  Serial.println(gameScore);
  Correct = map(gameScore, 0, 30, 0, 9);

  for(int i = 0; i <= Correct; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(300);
  }
  for(int i = Correct+1; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
    delay(300);
  }

  delay(3000);
  CircuitPlayground.clearPixels();
  gameState = 0;
}

void makeSound(int frequency, int time) {

  toneTimer.start(time, AsyncDelay::MILLIS);

  analogWrite(speakerPin, gameVolume);  // Set volume
  tone(speakerPin, frequency);      // Start tone
  

}