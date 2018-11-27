 /*
  * Project: Simple Game
  * Team: 810
  * Part: qxm
  * 
  */

#include <M5Stack.h>

bool inGame = false;

int x = 0, y = 21;
int energy = 0;
bool falling = false;

const int wall_n = 3;
const int wall_x[] = {-100, 200, 400};
const int wall_y1[] = {0, 0, 0};
const int wall_y2[] = {500, 50, 500};

const int ground_n = 2;
const int ground_y[] = {0, 50};
const int ground_x1[] = {-100, 200};
const int ground_x2[] = {400, 400};

const int goal_x = 350;
const int goal_y = 71;

int fr_x = 0, fr_y = 0;
int fr_x_min, fr_x_max, fr_y_min, fr_y_max;

void adjustFrame() {
  int sw = M5.Lcd.width();
  if (x - fr_x > 0.6 * sw) {fr_x = x - 0.6 * sw;}
  else if (x - fr_x < 0.4 * sw) {fr_x = x - 0.4 * sw;}

  int sh = M5.Lcd.height();
  if (y - fr_y > 0.6 * sh) {fr_y = y - 0.6 * sh;}
  else if (y - fr_y < 0.4 * sh) {fr_y = y - 0.4 * sh;}

  if (fr_x < fr_x_min) {fr_x = fr_x_min;}
  else if (fr_x > fr_x_max) {fr_x = fr_x_max;}

  if (fr_y < fr_y_min) {fr_y = fr_y_min;}
  else if (fr_y > fr_y_max) {fr_y = fr_y_max;}
}

int old_chara_lcd_x = -1, old_chara_lcd_y = -1;
void drawChara(int lcd_x, int lcd_y, int tft_color = TFT_CYAN) {
  if (!(lcd_x == old_chara_lcd_x && lcd_y == old_chara_lcd_y)) {
    M5.Lcd.drawCircle(old_chara_lcd_x + 5, old_chara_lcd_y + 4, 4, TFT_BLACK);
    M5.Lcd.drawLine(old_chara_lcd_x + 5, old_chara_lcd_y + 8, old_chara_lcd_x + 5, old_chara_lcd_y + 15, TFT_BLACK);
    M5.Lcd.drawLine(old_chara_lcd_x, old_chara_lcd_y + 11, old_chara_lcd_x + 10, old_chara_lcd_y + 11, TFT_BLACK);
    M5.Lcd.drawLine(old_chara_lcd_x + 5, old_chara_lcd_y + 15, old_chara_lcd_x, old_chara_lcd_y + 20, TFT_BLACK);
    M5.Lcd.drawLine(old_chara_lcd_x + 5, old_chara_lcd_y + 15, old_chara_lcd_x + 10, old_chara_lcd_y + 20, TFT_BLACK);
    
    M5.Lcd.drawCircle(lcd_x + 5, lcd_y + 4, 4, tft_color);
    M5.Lcd.drawLine(lcd_x + 5, lcd_y + 8, lcd_x + 5, lcd_y + 15, tft_color);
    M5.Lcd.drawLine(lcd_x, lcd_y + 11, lcd_x + 10, lcd_y + 11, tft_color);
    M5.Lcd.drawLine(lcd_x + 5, lcd_y + 15, lcd_x, lcd_y + 20, tft_color);
    M5.Lcd.drawLine(lcd_x + 5, lcd_y + 15, lcd_x + 10, lcd_y + 20, tft_color);

    old_chara_lcd_x = lcd_x;
    old_chara_lcd_y = lcd_y;
  }
}

int old_goal_lcd_x = -1, old_goal_lcd_y = -1;
void drawGoal(int lcd_x, int lcd_y, int tft_color = TFT_RED) {
  if (!(lcd_x == old_goal_lcd_x && lcd_y == old_goal_lcd_y)) {
    M5.Lcd.drawLine(old_goal_lcd_x, old_goal_lcd_y, old_goal_lcd_x, old_goal_lcd_y + 20, TFT_BLACK);
    M5.Lcd.drawLine(old_goal_lcd_x, old_goal_lcd_y, old_goal_lcd_x + 10, old_goal_lcd_y + 5, TFT_BLACK);
    M5.Lcd.drawLine(old_goal_lcd_x, old_goal_lcd_y + 10, old_goal_lcd_x + 10, old_goal_lcd_y + 5, TFT_BLACK);
    
    M5.Lcd.drawLine(lcd_x, lcd_y, lcd_x, lcd_y + 20, tft_color);
    M5.Lcd.drawLine(lcd_x, lcd_y, lcd_x + 10, lcd_y + 5, tft_color);
    M5.Lcd.drawLine(lcd_x, lcd_y + 10, lcd_x + 10, lcd_y + 5, tft_color);
    
    old_goal_lcd_x = lcd_x;
    old_goal_lcd_y = lcd_y;
  }
}

int* old_wall_lcd_x;
int* old_wall_lcd_y1;
int* old_wall_lcd_y2;
void drawWalls(int fr_x, int fr_y, int tft_color = TFT_YELLOW) {
  for (int i = 0; i < wall_n; i++) {
    int lcd_x = wall_x[i] - fr_x, lcd_y1 = fr_y - wall_y2[i], lcd_y2 = fr_y - wall_y1[i];
    if (lcd_x < 0 || lcd_x > M5.Lcd.width() || lcd_y1 > M5.Lcd.height() || lcd_y2 < 0) {
      if (old_wall_lcd_x[i] != -1 && old_wall_lcd_y1[i] != -1 && old_wall_lcd_y2[i] != -1) 
      {M5.Lcd.drawLine(old_wall_lcd_x[i], old_wall_lcd_y1[i], old_wall_lcd_x[i], old_wall_lcd_y2[i], TFT_BLACK);}
      old_wall_lcd_x[i] = -1;
      old_wall_lcd_y1[i] = -1;
      old_wall_lcd_y2[i] = -1;
    }
    else {
      if (lcd_y1 < 0) {lcd_y1 = 0;}
      if (lcd_y2 > M5.Lcd.height()) {lcd_y2 = M5.Lcd.height();}
      if (!(lcd_x == old_wall_lcd_x[i] && lcd_y1 == old_wall_lcd_y1[i] && lcd_y2 == old_wall_lcd_y2[i])) {
        if (old_wall_lcd_x[i] != -1 && old_wall_lcd_y1[i] != -1 && old_wall_lcd_y2[i] != -1) 
        {M5.Lcd.drawLine(old_wall_lcd_x[i], old_wall_lcd_y1[i], old_wall_lcd_x[i], old_wall_lcd_y2[i], TFT_BLACK);}
        old_wall_lcd_x[i] = lcd_x;
        old_wall_lcd_y1[i] = lcd_y1;
        old_wall_lcd_y2[i] = lcd_y2;
      }
      M5.Lcd.drawLine(lcd_x, lcd_y1, lcd_x, lcd_y2, tft_color);
    }
  }
}

int* old_ground_lcd_y;
int* old_ground_lcd_x1;
int* old_ground_lcd_x2;
void drawGrounds(int fr_x, int fr_y, int tft_color = TFT_YELLOW) {
  for (int i = 0; i < ground_n; i++) {
    int lcd_y = fr_y - ground_y[i], lcd_x1 = ground_x1[i] - fr_x, lcd_x2 = ground_x2[i] - fr_x;
    if (lcd_y < 0 || lcd_y > M5.Lcd.height() || lcd_x1 > M5.Lcd.width() || lcd_x2 < 0) {
      if (old_ground_lcd_y[i] != -1 && old_ground_lcd_x1[i] != -1 && old_ground_lcd_x2[i] != -1) 
      {M5.Lcd.drawLine(old_ground_lcd_x1[i], old_ground_lcd_y[i], old_ground_lcd_x2[i], old_ground_lcd_y[i], TFT_BLACK);}
      old_ground_lcd_y[i] = -1;
      old_ground_lcd_x1[i] = -1;
      old_ground_lcd_x2[i] = -1;
    }
    else {
      if (lcd_x1 < 0) {lcd_x1 = 0;}
      if (lcd_x2 > M5.Lcd.width()) {lcd_x2 = M5.Lcd.width();}
      if (!(lcd_y == old_ground_lcd_y[i] && lcd_x1 == old_ground_lcd_x1[i] && lcd_x2 == old_ground_lcd_x2[i])) {
        if (old_ground_lcd_y[i] != -1 && old_ground_lcd_x1[i] != -1 && old_ground_lcd_x2[i] != -1) 
        {M5.Lcd.drawLine(old_ground_lcd_x1[i], old_ground_lcd_y[i], old_ground_lcd_x2[i], old_ground_lcd_y[i], TFT_BLACK);}
        old_ground_lcd_y[i] = lcd_y;
        old_ground_lcd_x1[i] = lcd_x1;
        old_ground_lcd_x2[i] = lcd_x2;
      }
      M5.Lcd.drawLine(lcd_x1, lcd_y, lcd_x2, lcd_y, tft_color);
    }
  }
}

void draw() {
  /* Some coordinates display
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_CYAN);
  int fh = M5.Lcd.fontHeight(1);
  int sw = M5.Lcd.width();
  int sh = M5.Lcd.height();
  int cx = sw / 2, cy = 0;
  M5.Lcd.fillRect(0, 0, sw, cy + fh, TFT_CYAN);
  M5.Lcd.setTextDatum(TC_DATUM);
  char str[256];
  sprintf(str, "x: %d, y: %d, frame: %d, %d", x, y, fr_x, fr_y);
  M5.Lcd.drawString(str, cx, cy); 
  */

  adjustFrame();
  drawChara(x - fr_x, fr_y - y);
  drawGoal(goal_x - fr_x, fr_y - goal_y);
  drawWalls(fr_x, fr_y);
  drawGrounds(fr_x, fr_y);
}

bool goalReached(int x, int y) {
  return x - goal_x <= 10 && goal_x - x <= 10  && y - goal_y <= 20 && goal_y - y <= 20;
}

bool leftIsWall(int x, int y) {
  for (int i = 0; i < wall_n; i++) {
    if(x == wall_x[i] + 1 && y >= wall_y1[i] && y - 20 <= wall_y2[i])
    {return true;}
  }
  return false; 
}

bool rightIsWall(int x, int y) {
  for (int i = 0; i < wall_n; i++) {
    if(x == wall_x[i] - 11 && y >= wall_y1[i] && y - 20 <= wall_y2[i])
    {return true;}
  }
  return false; 
}

bool botIsGround(int x, int y) {
  for (int i = 0; i < ground_n; i++) {
    if(y == ground_y[i] + 21 && x + 10 >= ground_x1[i] && x <= ground_x2[i])
    {return true;}
  }
  return false;
}

bool topIsGround(int x, int y) {
  for (int i = 0; i < ground_n; i++) {
    if(y == ground_y[i] - 1 && x + 10 >= ground_x1[i] && x <= ground_x2[i])
    {return true;}
  }
  return false;
}

void charaMoveLeft() {
  while (!M5.BtnA.isReleased() && inGame) {
    if(!leftIsWall(x, y)) {x--;}
    if(!botIsGround(x, y)) {falling = true; energy = 0; charaFallLeft();}
    else {
      if (M5.BtnC.wasPressed()) {energy = 100; charaMoveLeftJump();}
    }
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
}

void charaMoveLeftJump() {
  while (!M5.BtnA.isReleased() && !M5.BtnC.isReleased() && inGame) {
    if(!leftIsWall(x, y)) {x--;}
    if(energy > 0 && !topIsGround(x, y)) {y++; energy--;}
    else {falling = true; energy = 0; charaFallLeft();}
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
  if (M5.BtnC.wasReleased()) {
    if (!botIsGround(x, y)) {
      falling = true; energy = 0;
      if (M5.BtnA.wasReleased()) {charaFall();}
      else {charaFallLeft();}
    }
  }
  else if (M5.BtnA.wasReleased()){
    charaJump();
  }
}

void charaMoveRight() {
  while (!M5.BtnB.isReleased() && inGame) {
    if(!rightIsWall(x, y)) {x++;}
    if(!botIsGround(x, y)) {falling = true; energy = 0; charaFallRight();}
    else {
      if (M5.BtnC.wasPressed()) {energy = 100; charaMoveRightJump();}
    }
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
}

void charaMoveRightJump() {
  while (!M5.BtnB.isReleased() && !M5.BtnC.isReleased() && inGame) {
    if(!rightIsWall(x, y)) {x++;}
    if(energy > 0 && !topIsGround(x, y)) {y++; energy--;}
    else {falling = true; energy = 0; charaFallRight();}
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
  if (M5.BtnC.wasReleased()) {
    if (!botIsGround(x, y)) {
      falling = true; energy = 0;
      if (M5.BtnB.wasReleased()) {charaFall();}
      else {charaFallRight();}
    }
  }
  else if (M5.BtnB.wasReleased()){
    charaJump();
  }
}

void charaJump() {
  while (!M5.BtnC.isReleased() && inGame) {
    if(energy > 0 && !topIsGround(x, y)) {
      y++; energy--;
      if (M5.BtnA.isPressed()) {charaMoveLeftJump();}
      else if (M5.BtnB.isPressed()) {charaMoveRightJump();}
    }
    else {falling = true; energy = 0; charaFall();}
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
  if (!botIsGround(x, y)) {falling = true; energy = 0; charaFall();}
}

void charaFall() {
  while (falling && inGame) {
    if (M5.BtnA.isPressed()) {charaFallLeft();}
    else if (M5.BtnB.isPressed()) {charaFallRight();}
    else {
      if (botIsGround(x, y)) {falling = false;}
      else {y--;}
      if (goalReached(x, y)) {inGame = false;}
      draw();
      M5.update();
      delay(10);
    }
  }
}

void charaFallLeft() {
  while (!M5.BtnA.isReleased() && falling && inGame) {
    if(!leftIsWall(x, y)) {x--;}
    if (botIsGround(x, y)) {falling = false;}
    else {y--;}
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
  if (falling) {charaFall();}
  else if (!M5.BtnA.isReleased()) {charaMoveLeft();}
}

void charaFallRight() {
  while (!M5.BtnB.isReleased() && falling && inGame) {
    if(!rightIsWall(x, y)) {x++;} 
    if (botIsGround(x, y)) {falling = false;}
    else {y--;}
    if (goalReached(x, y)) {inGame = false;}
    draw();
    M5.update();
    delay(10);
  }
  if (falling) {charaFall();}
  else if (!M5.BtnB.isReleased()) {charaMoveRight();}
}

void setup() {
  M5.begin();
  inGame = true;
  
  fr_x_min = wall_x[0] - 50;
  fr_x_max = wall_x[wall_n - 1] + 50 - M5.Lcd.width();
  if (fr_x_max < fr_x_min) fr_x_max = fr_x_min;

  fr_y_min = ground_y[0] - 50 + M5.Lcd.height();
  fr_y_max = ground_y[ground_n - 1] + 50;
  if (fr_y_max < fr_y_min) fr_y_max = fr_y_min;
  
  old_wall_lcd_x = (int*) malloc(wall_n * sizeof(int));
  old_wall_lcd_y1 = (int*) malloc(wall_n * sizeof(int));
  old_wall_lcd_y2 = (int*) malloc(wall_n * sizeof(int));
  
  old_ground_lcd_y = (int*) malloc(ground_n * sizeof(int));
  old_ground_lcd_x1 = (int*) malloc(ground_n * sizeof(int));
  old_ground_lcd_x2 = (int*) malloc(ground_n * sizeof(int));

  memset(old_wall_lcd_x, -1, wall_n * sizeof(int));
  memset(old_wall_lcd_y1, -1, wall_n * sizeof(int));
  memset(old_wall_lcd_y2, -1, wall_n * sizeof(int));

  memset(old_ground_lcd_y, -1, ground_n * sizeof(int));
  memset(old_ground_lcd_x1, -1, ground_n * sizeof(int));
  memset(old_ground_lcd_x2, -1, ground_n * sizeof(int));

  draw();
}

void loop() {
  if (inGame) {
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
  else {
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
      x = 0; y = 21;
      inGame = true;
      draw();
      M5.update();
    }
    else {
      M5.update();
    }
  }
}
