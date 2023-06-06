#include <TFT_eSPI.h>
#include <SPI.h>
#include <Keypad.h>

#define TFT_CS 15
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18
#define buzzer 5
TFT_eSPI tft = TFT_eSPI();

uint16_t y = 0;
uint16_t x = tft.width() / 2;
uint16_t h = tft.height() / 2;
uint16_t count = 0;

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
uint8_t rowPins[ROWS] = { 27, 14, 14, 13 };
uint8_t colPins[COLS] = { 32, 33, 25, 26 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char key_press;

volatile uint8_t page_count = 0;
volatile uint8_t welcome_steps = 0;
volatile uint8_t level_steps = 0;
volatile uint8_t game_step = 0;
volatile uint8_t endGame_step = 0;


//////////////////////////////////////////////////////
const int screen_width = 240;
const int screen_height = 320;
const int screen_margin = 5;
const int player_size = 20;

int counter = 0;

bool isDead = false,  isLost = false;


// the starting pos
int starting_x = (screen_width - player_size) / 2, starting_y = (screen_height - player_size * 3);


// place the player to the starting pos
int player_x = starting_x, player_y = starting_y;

int greenSq_x, greenSq_y;

int square1_x = 152, square1_y = 197;

int square2_x = 68, square2_y = 113;

int score = 0;

int lvl = 1;

int _counter = 0, countdown = 0, deadLine = 0;
////////////////////////////////////////////////////


void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  welcome();
}

void loop() {


  if (page_count == 0) {
    welcome();
    level_steps = 0;
  }

  if (page_count == 1) {
    level();
    welcome_steps = 0;
  }

  if (page_count == 2) {
    Game();
    level_steps = 0;
  }

  if (page_count == 3) {
    EndGame();
    game_step = 0;
  }

  if (page_count == 4) {
    page_count = 0;
    endGame_step = 0;
  }

}

char scan() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
  return key;
}

void welcome() {
  if (welcome_steps == 0) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(60, 10, 4);
    tft.setTextColor(TFT_CYAN);
    tft.print("Welcome!!");
    

    tft.setTextColor(TFT_CYAN);
    tft.setCursor(50, 160);
      tft.print("  Made by:\n    Ahmad & Yazzed");

      tft.setCursor(30, 260);
      tft.print("Press 'A' to start");

      
    
    
    
    welcome_steps++;
  }

  if(welcome_steps == 1){
    key_press = scan();

    if (key_press == 'A') {
     
      key_press = NO_KEY;
      page_count++;
    }
  }

}

void level() {
  if (level_steps == 0) {
    tft.fillScreen(TFT_BLACK);
    level_steps++;
  }
  if (level_steps == 1) {
    level_steps++;
    
    tft.setCursor(50, 10, 4);
    tft.setTextColor(TFT_WHITE);
    tft.println("1: Hard");

    tft.setCursor(50, 50, 4);
    tft.println("2: Normal");

    tft.setCursor(50, 90, 4);
    tft.println("3: Easy");

    int y1 = 20, y2 = 5, y3 = 30;
    tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_RED);
    lvl = 1;
    while(true)
    {
      
      char key = keypad.getKey(); // read the input from the keypad
      //if (key != NO_KEY) { // check if a key was pressed

        switch (key) {
        case '1':
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_BLACK);
          y1 = 20;
          y2 = 5;
          y3 = 30;
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_RED);
          lvl = 1;
        break;
        case '2':
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_BLACK);
          y1 = 60;
          y2 = 45;
          y3 = 70;
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_RED);
          lvl = 2;
        break;
        case '3':
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_BLACK);
          y1 = 100;
          y2 = 85;
          y3 = 110;
          tft.fillTriangle(180,y1 ,220, y2,220, y3,TFT_RED);
          lvl = 3;
        break;
        case 'A':
       
          // incress the screen counter to go to the game 
          page_count = 2;
        break;
        }
      //}
      if(key == 'A')break;
    }
  }
}

void Game(){
  
  if(game_step == 0)
  {
    counter = 0;

    isDead = false;
    isLost = false;

    // the starting pos
    starting_x = (screen_width - player_size) / 2;
    starting_y = (screen_height - player_size * 3);
    // place the player to the starting pos
    player_x = starting_x;
    player_y = starting_y;

    greenSq_x;
    greenSq_y;

    square1_x = 152;
    square1_y = 197;

    square2_x = 68;
    square2_y = 113;

    score = 0;

    switch(lvl){
      case 1: 
        countdown = 100;
        break;
      case 2:
        countdown = 150;
        break;
      case 3:
        countdown = 200;
        break; 
    }

    tft.fillScreen(TFT_BLACK);

    // Draw the red edges around the screen
    tft.fillRect(0, 0, screen_width, screen_margin, TFT_WHITE); // top
    tft.fillRect(0, screen_height - 38, screen_width, screen_margin, TFT_WHITE); // bottom
    tft.fillRect(0, screen_margin, screen_margin, (screen_height - 2 * screen_margin) - 33, TFT_WHITE); // left
    tft.fillRect(screen_width - screen_margin, screen_margin, screen_margin, (screen_height - 2 * screen_margin) - 33, TFT_WHITE); // right

    // draw the first green square
    GreenSp();
    tft.fillRect(greenSq_x, greenSq_y, player_size, player_size, TFT_GREEN);
    
    // print score
    tft.setCursor(5, 300);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.println(score);

    // draw player
    tft.fillRect(player_x, player_y, player_size, player_size, TFT_BLUE);

    // draw enemy
    tft.fillRect(square1_x, square1_y, player_size, player_size, TFT_RED);
    tft.fillRect(square2_x, square2_y, player_size, player_size, TFT_RED);

    // print countdown
    tft.fillRect(200, 300, 50, 50, TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setCursor(200, 300);
    tft.setTextSize(1);
    tft.println(countdown);

    game_step++;
  }

  

  while(game_step == 1){
    MovementControle();

    Borders();
      
    Score();

    Enemy();

    Countdown();

  }

  page_count++;

}

void Countdown(){

  _counter++;
  if(_counter == 35000){
    _counter = 0;
    countdown--;

    // print the countdown
    tft.fillRect(200, 300, 50, 50, TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setCursor(200, 300);
    tft.setTextSize(1);
    tft.println(countdown);
  }

  if(countdown <= 0){
    game_step++;
    isLost = true;
  }

}

void EndGame(){

  if(endGame_step == 0){
    isDead = true;
    if(counter <= 1){
      counter++;
      tft.fillScreen(TFT_BLACK);
    }
    if(isLost){

      tft.setCursor(30, 120);
      tft.setTextColor(TFT_RED);
      tft.setTextSize(1);
      tft.println("GAME OVER!!");

      tft.setCursor(20, 200);
      tft.println("Press A to replay");

      tft.setCursor(20, 30);
      tft.print("Your score is :");

      tft.setCursor(190, 30);
      tft.print(score);


    }
    else{

      tft.setCursor(50, 30);
      tft.setTextColor(TFT_GREEN);
      tft.setTextSize(1);
      tft.println("YOU WON!!");

      tft.setTextColor(TFT_WHITE);
      tft.setCursor(30, 260);
      tft.print("Press 'A' to start");

      tft.setCursor(30, 160);
      tft.print("Your score is :");

      tft.setCursor(195, 160);
      tft.print(score);

    }
    endGame_step++;
  }
  if(endGame_step == 1){
  
    char key = keypad.getKey();

    if (key == 'A') {
      page_count++;
      key_press = NO_KEY;
    }

  }
}

void Enemy(){
  if(square1_x == player_x && square1_y == player_y){
    game_step++;
    isLost = true;
  } 
  if(square2_x == player_x && square2_y == player_y){
    game_step++;
    isLost = true;
  }
  
}

void Score(){
  // check if the player tuched the green square
  if(player_x == greenSq_x && player_y == greenSq_y){
    
    GreenSp();

    // incress the score
    score++;

    // print the new score
    tft.fillRect(5, 300, 50, 50, TFT_BLACK);
    tft.setCursor(5, 300);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.println(score);

    // spawn the new green square
    delay(500);
    tft.fillRect(greenSq_x, greenSq_y, player_size, player_size, TFT_GREEN);

    if(score >= 3){
      game_step++;
    }

  }



}

void GreenSp(){
  bool overLap;

  
  do{
    // genrate a random number for x and y
    int randX = random(1, 216);
    int randY = random(1, 261);

    // assigen the random number to x and y
    greenSq_x = (randX - (randX%21)) + 5;
    greenSq_y = (randY - (randY%21)) + 8;

    if(square1_x != greenSq_x && square1_y != greenSq_y) overLap = false;
    if(square2_x != greenSq_x && square2_y != greenSq_y) overLap = false;

  }while(overLap == true);

    
}


void MovementControle(){
  if(!isDead){
    
    char key = keypad.getKey(); // read the input from the keypad
    if (key != NO_KEY) { // check if a key was pressed
      
      switch (key) {
        case '2':
        buzr();// if the '2' key is pressed, move the circle up
          MoveU();
          break;
        case '4':
        buzr();// if the '4' key is pressed, move the circle left
          MoveL();
          break;
        case '6':
        buzr();// if the '6' key is pressed, move the circle right
          MoveR();
          break;
        case '5':
        buzr();// if the '8' key is pressed, move the circle down
          MoveD();
          break;
      }
    }
  }
}

void Borders(){
  // Check if the player has reached the top edge
  if (player_y < 0 || player_y > 280 || player_x < 0 || player_x > 230) {
    isLost = true;
    game_step++;
  }

}

void MoveU(){

  player_y -= player_size + 1;

  // Draw the square at its current position
  tft.fillRect(player_x, player_y, player_size, player_size, TFT_BLUE);
  tft.fillRect(player_x, player_y + 21, player_size, player_size, TFT_BLACK);

}

void MoveD(){
  // move player to the next pos
  player_y += player_size + 1;

  // Draw the player at its current pos
  tft.fillRect(player_x, player_y, player_size, player_size, TFT_BLUE);
  tft.fillRect(player_x, player_y - 21, player_size, player_size, TFT_BLACK);

}

void MoveL(){
  // move player to the next pos
  player_x -= player_size + 1;

  // Draw the player at its current pos
  tft.fillRect(player_x, player_y, player_size, player_size, TFT_BLUE);
  tft.fillRect(player_x + 21, player_y, player_size, player_size, TFT_BLACK);

}

void MoveR(){
  // move player to the next pos
  player_x += player_size + 1;

  // Draw the player at its current pos
  tft.fillRect(player_x, player_y, player_size, player_size, TFT_BLUE);
  tft.fillRect(player_x - 21, player_y, player_size, player_size, TFT_BLACK);

}
void buzr (){

  tone(buzzer,1915);
  delay(5);
  tone(buzzer,0);


}
