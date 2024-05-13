#include <Gamebuino-Meta.h>

const int GRID_SIZE = 8;
const int CELL_SIZE = 10;
enum GameState {
    MENU,
    GAME,
    GAME_OVER
};
int foodX, foodY;
int snakeX[256], snakeY[256];
int snakeLength = 3;
int direction = 1; // 0: haut, 1: droite, 2: bas, 3: gauche
bool gameOver = false;
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
      drawGrid();
      handleInput();
      moveSnake();
      checkFoodCollision();
      drawSnake();
      drawFood();
      delay(100);
      break;
    case GAME_OVER:
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
  gb.display.setCursor(20, 20);
  gb.display.print("Snake Game");
  gb.display.setCursor(20, 30);
  gb.display.print("Press A to Start");
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
  for (int i = 0; i <= GRID_SIZE; i++) {
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
  gb.display.fillRect(foodX * CELL_SIZE, foodY * CELL_SIZE, CELL_SIZE, CELL_SIZE);
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
    gameOver = true;
    gameState = GAME_OVER;
    return;
  }

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  snakeX[0] = nextX;
  snakeY[0] = nextY;
}

void drawSnake() {
  gb.display.setColor(BLUE);
  for (int i = 0; i < snakeLength; i++) {
    gb.display.fillRect(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE);
  }
}

void checkFoodCollision() {
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    placeFood();
  }
}
void displayGameOver() {
  gb.display.clear();
  gb.display.setColor(WHITE);
  gb.display.setCursor(20, 20);
  gb.display.print("GAME OVER");
  gb.display.setCursor(20, 30);
  gb.display.print("Score: ");
  gb.display.print(snakeLength - 3);
  gb.display.setCursor(20, 40);
  gb.display.print("A: Restart");
  gb.display.setCursor(20, 50);
  gb.display.print("B: Menu");
}

void handleGameOverInput() {
  if (gb.buttons.pressed(BUTTON_A)) {
    resetGame();
    gameState = GAME;
  } else if (gb.buttons.pressed(BUTTON_B)) {
    gameState = MENU;
  }
}
