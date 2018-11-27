 /*
  * Project: Simple Game
  * Team: 810
  * Part: qxm
  * 
  */

#include <M5Stack.h>


#include "network.h"

int x = 0, y = 1;
int energy = 0;
bool falling = false;

void drawCount() {
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  int fh = M5.Lcd.fontHeight(1);
  int sw = M5.Lcd.width();
  int sh = M5.Lcd.height();
  int cx1 = sw / 2 - 60, cx2 = sw / 2 + 60, cy = sh / 2;
  M5.Lcd.fillRect(0, cy - fh / 2, 320, fh, TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawNumber(x, cx1, cy);
  M5.Lcd.drawNumber(y, cx2, cy);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  drawCount();
  //WiFiTest();
}

bool leftIsWall(int x) {
  return x == -999; 
}

bool rightIsWall(int x) {
  return x == 999;
}

bool botIsGround(int y) {
  return y == 1;
}

void charaMoveLeft() {
  while (!M5.BtnA.isReleased()) {
    if(!leftIsWall(x)) {x--;}
    if(!botIsGround(y)) {falling = true; charaFallLeft();}
    else {
      if (M5.BtnC.wasPressed()) {energy = 100; charaMoveLeftJump();}
    }
    drawCount();
    M5.update();
    delay(10);
  }
}

void charaMoveLeftJump() {
  while (!M5.BtnA.isReleased() && !M5.BtnC.isReleased()) {
    if(!leftIsWall(x)) x--; 
    if(energy > 0) {y++; energy--;}
    else {falling = true; charaFallLeft();}
    drawCount();
    M5.update();
    delay(10);
  }
  if (M5.BtnC.wasReleased()) {
    if (!botIsGround(y)) {
      falling = true; 
      if (M5.BtnA.wasReleased()) {charaFall();}
      else {charaFallLeft();}
    }
  }
  else if (M5.BtnA.wasReleased()){
    charaJump();
  }
}

void charaMoveRight() {
  while (!M5.BtnB.isReleased()) {
    if(!rightIsWall(x)) {x++;}
    if(!botIsGround(y)) {falling = true; charaFallRight();}
    else {
      if (M5.BtnC.wasPressed()) {energy = 100; charaMoveRightJump();}
    }
    drawCount();
    M5.update();
    delay(10);
  }
}

void charaMoveRightJump() {
  while (!M5.BtnB.isReleased() && !M5.BtnC.isReleased()) {
    if(!rightIsWall(x)) {x++;}
    if(energy > 0) {y++; energy--;}
    else {falling = true; charaFallRight();}
    drawCount();
    M5.update();
    delay(10);
  }
  if (M5.BtnC.wasReleased()) {
    if (!botIsGround(y)) {
      falling = true;
      if (M5.BtnB.wasReleased()) {charaFall();}
      else {charaFallRight();}
    }
  }
  else if (M5.BtnB.wasReleased()){
    charaJump();
  }
}

void charaJump() {
  while (!M5.BtnC.isReleased()) {
    if(energy > 0) {y++; energy--;}
    else {falling = true;}
    if(falling) {charaFall();}
    else {
      if (M5.BtnA.isPressed()) {charaMoveLeftJump();}
      else if (M5.BtnB.isPressed()) {charaMoveRightJump();}
    }
    drawCount();
    M5.update();
    delay(10);
  }
  if (!botIsGround(y)) {falling = true; charaFall();}
}

void charaFall() {
  while (falling) {
    if (M5.BtnA.isPressed()) {charaFallLeft();}
    else if (M5.BtnB.isPressed()) {charaFallRight();}
    else {
      if (botIsGround(y)) {falling = false;}
      else {y--;}
      drawCount();
      M5.update();
      delay(10);
    }
  }
}

void charaFallLeft() {
  while (!M5.BtnA.isReleased() && falling) {
    if(!leftIsWall(x)) x--;
    if (botIsGround(y)) {falling = false;}
    else {y--;}
    drawCount();
    M5.update();
    delay(10);
  }
  if (falling) {charaFall();}
  else if (!M5.BtnA.isReleased()) {charaMoveLeft();}
}

void charaFallRight() {
  while (!M5.BtnB.isReleased() && falling) {
    if(!rightIsWall(x)) x++; 
    if (botIsGround(y)) {falling = false;}
    else {y--;}
    drawCount();
    M5.update();
    delay(10);
  }
  if (falling) {charaFall();}
  else if (!M5.BtnB.isReleased()) {charaMoveRight();}
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    charaMoveLeft();
  }
  else if (M5.BtnB.wasPressed()) {
    charaMoveRight();
  }
  else if (M5.BtnC.wasPressed() && !falling) {
    energy = 100;
    charaJump();
  }
  else if (falling) {
    charaFall();
  }
  else {
    M5.update();
  }
}
