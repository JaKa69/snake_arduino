#include <Gamebuino-Meta.h>

const int GRID_SIZE = 8;
const int CELL_SIZE = 8;
enum GameState {
    MENU,
    GAME,
    GAME_OVER
};
const int SNAKE_SPEED = 5;
int foodX, foodY;
const uint16_t appleData[] = {
    8,      // frame width
    8,      // frame height
    1,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x4664, 0x4664, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xea64, 0xd285, 0x4664, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xeaa5, 0xea64, 0xe223, 0xe223, 0xf81f, 0xf81f,
    0xf81f, 0xe223, 0xea64, 0xea64, 0xebab, 0xeae7, 0xe223, 0xf81f,
    0xf81f, 0xe223, 0xe223, 0xe223, 0xebab, 0xebab, 0xe223, 0xf81f,
    0xf81f, 0xe223, 0xe223, 0xe223, 0xea64, 0xeb49, 0xe223, 0xf81f,
    0xf81f, 0xf81f, 0xe223, 0xe223, 0xe223, 0xe223, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xe223, 0xe223, 0xf81f, 0xf81f, 0xf81f
};
Image appleImg(appleData);

int snakeX[256], snakeY[256];
int snakeLength = 3;
const uint16_t snakeHeadData[] = {
    8,      // frame width
    8,      // frame height
    1,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode
    0xf81f, 0x39df, 0x4a5e, 0x003f, 0x087f, 0x6b1d, 0x421f, 0xf81f,
    0x317f, 0xffff, 0xffff, 0xce7f, 0xce7f, 0xffff, 0xffff, 0x293f,
    0x4a5f, 0xffff, 0x0020, 0xce5f, 0xce5f, 0x0000, 0xffff, 0x5ade,
    0x085f, 0xffff, 0xffff, 0xce7f, 0xce5f, 0xffff, 0xffff, 0x003f,
    0x003f, 0xce5f, 0xce5f, 0x087f, 0x003f, 0xb59f, 0xb59f, 0x003f,
    0x18df, 0x003f, 0x18bf, 0x18bf, 0x001f, 0x085f, 0x085f, 0x003f,
    0x109f, 0x003f, 0x085f, 0x003f, 0x001f, 0x083f, 0x003f, 0x003f,
    0xf81f, 0x087f, 0x109f, 0x085f, 0x109f, 0x003f, 0x085f, 0xf81f
};
Image snakeHeadImg(snakeHeadData);
const uint16_t snakeBodyData[] = {
    8,      // frame width
    8,      // frame height
    1,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode
    0xf79f, 0x087f, 0x003f, 0x085f, 0x085f, 0x085f, 0x085f, 0xffff,
    0x003f, 0xffff, 0x001f, 0x003f, 0x003f, 0x003f, 0xffdf, 0x211f,
    0x003f, 0x001f, 0xffff, 0x529f, 0x003f, 0xffff, 0x003f, 0x087f,
    0x003f, 0x001f, 0x001f, 0xffff, 0xdeff, 0x52bf, 0x001f, 0x003f,
    0x003f, 0x001f, 0x841f, 0xef7f, 0xf7bf, 0x003f, 0x001f, 0x003f,
    0x003f, 0x003f, 0xffff, 0x003f, 0x841f, 0xffff, 0x001f, 0x001f,
    0x109f, 0xffff, 0x087f, 0x003f, 0x001f, 0x003f, 0xffff, 0x001f,
    0xffdf, 0x18df, 0x003f, 0x001f, 0x003f, 0x003f, 0x087f, 0xffff

};
Image snakeBodyImg(snakeBodyData);

int direction = 1; // 0: haut, 1: droite, 2: bas, 3: gauche
int score = 0;
int bestScore = 0;
GameState gameState;

void setup() {
  gb.begin();
  gameState = MENU;
}

void loop() {
  while (!gb.update());
  switch (gameState) {
    case MENU:
      drawMenu();
      handleMenuInput();
      break;
    case GAME:
      handleInput();
      drawGrid();
      if (gb.frameCount % SNAKE_SPEED == 0) {
          moveSnake();
          checkFoodCollision();
      }
      drawSnake();
      drawFood();
      displayScore();
      break;
    case GAME_OVER:
      bestScore = (score > bestScore) ? score : bestScore;
      displayGameOver();
      handleGameOverInput();
      break;
  }
}

void resetGame() {
  snakeLength = 3;
  direction = 1;
  placeFood();
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = snakeLength - i - 1;
    snakeY[i] = 0;
  }
}

void drawMenu() {
  gb.display.clear();
  gb.display.setColor(WHITE);
  gb.display.setCursor(15, 20);
  gb.display.print("Snake Game");
  gb.display.setCursor(15, 30);
  gb.display.print("Press A to Start");
  gb.display.setCursor(15, 40);
  gb.display.print("last best score: ");
  gb.display.setCursor(39.5, 50);
  gb.display.print(bestScore);
}

void handleMenuInput() {
  if (gb.buttons.pressed(BUTTON_A)) {
    resetGame();
    gameState = GAME;
  }
}

void drawGrid() {
  gb.display.clear();
  gb.display.setColor(GREEN);
  gb.display.fillRect(0, 0, GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE);

  gb.display.setColor(BLACK);
  for (int i = 0; i < GRID_SIZE; i++) {
    gb.display.drawLine(i * CELL_SIZE, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
    gb.display.drawLine(0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
  }
}

void handleInput() {
  if (gb.buttons.pressed(BUTTON_UP) && direction != 2) {
    direction = 0;
  }
  if (gb.buttons.pressed(BUTTON_RIGHT) && direction != 3) {
    direction = 1;
  }
  if (gb.buttons.pressed(BUTTON_DOWN) && direction != 0) {
    direction = 2;
  }
  if (gb.buttons.pressed(BUTTON_LEFT) && direction != 1) {
    direction = 3;
  }
}

void placeFood() {
  bool onSnake;
  do {
    onSnake = false;
    foodX = random(GRID_SIZE);
    foodY = random(GRID_SIZE);
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        onSnake = true;
        break;
      }
    }
  } while (onSnake);
}

void drawFood() {
  gb.display.setColor(RED);
  gb.display.drawImage(foodX * CELL_SIZE, foodY * CELL_SIZE, appleImg);
}

void moveSnake() {
  int nextX = snakeX[0];
  int nextY = snakeY[0];
  switch (direction) {
    case 0: nextY--; break;
    case 1: nextX++; break;
    case 2: nextY++; break;
    case 3: nextX--; break;
  }

  // Check for wall collisions
  if (nextX < 0 || nextY < 0 || nextX >= GRID_SIZE || nextY >= GRID_SIZE) {
    gameState = GAME_OVER;
    return;
  }

  // check for snake tail collision
  for (int i = 1; i < snakeLength; i++) {
        if (snakeX[i] == nextX && snakeY[i] == nextY) {
            gameState = GAME_OVER;
            return;
        }
    }

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  snakeX[0] = nextX;
  snakeY[0] = nextY;
}

void drawSnake() {
  gb.display.drawImage(snakeX[0] * CELL_SIZE, snakeY[0] * CELL_SIZE, snakeHeadImg);
  
  for (int i = 1; i < snakeLength; i++) {
    gb.display.drawImage(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, snakeBodyImg);
  }
}


void checkFoodCollision() {
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    placeFood();
    score++;
  }
}
void displayGameOver() {
  gb.display.clear();
  gb.display.setColor(WHITE);
  gb.display.setCursor(20, 20);
  gb.display.print("GAME OVER");
  gb.display.setCursor(20, 30);
  gb.display.print("Score: ");
  gb.display.print(score);
  gb.display.setCursor(20, 40);
  gb.display.print("Best Score: ");
  gb.display.print(bestScore);
  gb.display.setCursor(20, 50);
  gb.display.print("A: Restart");
  gb.display.setCursor(20, 60);
  gb.display.print("B: Menu");
}

void displayScore() {
  gb.display.setColor(WHITE);
  gb.display.setCursor(55, 5);
  gb.display.print("Score: ");
  gb.display.setCursor(65, 15);
  gb.display.print(score);
}

void handleGameOverInput() {
  if (gb.buttons.pressed(BUTTON_A)) {
    resetGame();
    gameState = GAME;
      score = 0;
  } else if (gb.buttons.pressed(BUTTON_B)) {
    gameState = MENU;
      score = 0;
  }
}
