#include <M5StickCPlus2.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

#define BACKGROUND_COLOR BLUE
#define BIRD_COLOR ORANGE
#define PIPE_COLOR GREEN
#define PIPE_HEAD_COLOR DARKGREEN
#define PIPE_END_COLOR DARKGREEN
#define TEXT_COLOR WHITE
#define BATTERY_COLOR BLUE
#define GRASS_COLOR DARKGREEN

int birdY = SCREEN_HEIGHT / 2;
int birdVelocity = 0;
int gravity = 1;
int pipeWidth = 20;
int pipeGap = 60;
int pipeX = SCREEN_WIDTH;
int pipeHeight = random(20, SCREEN_HEIGHT - pipeGap - 20);
int score = 0;
int highScore = 0;
bool gameOver = false;
bool gamePaused = false;

int lastUpdateTime = 0;

void drawBird() {
  M5.Lcd.fillRect(70, birdY, 15, 15, BIRD_COLOR);
}

void drawPipes() {
  M5.Lcd.fillRect(pipeX, 0, pipeWidth, pipeHeight, PIPE_COLOR);
  M5.Lcd.fillRect(pipeX - 5, pipeHeight - 10, pipeWidth + 10, 10, PIPE_HEAD_COLOR);
  M5.Lcd.fillRect(pipeX, pipeHeight + pipeGap, pipeWidth, SCREEN_HEIGHT - (pipeHeight + pipeGap), PIPE_COLOR);
  M5.Lcd.fillRect(pipeX - 5, pipeHeight + pipeGap, pipeWidth + 10, 10, PIPE_HEAD_COLOR);
}

void drawGrass() {
  M5.Lcd.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, GRASS_COLOR);
}

void updateGame() {
  if (gameOver || gamePaused) return;

  birdVelocity += gravity;
  birdY += birdVelocity;

  pipeX -= 2;
  if (pipeX + pipeWidth < 0) {
    pipeX = SCREEN_WIDTH;
    pipeHeight = random(20, SCREEN_HEIGHT - pipeGap - 20);
    score++;
  }

  if (birdY + 15 >= SCREEN_HEIGHT - 10 || (pipeX < 70 && pipeX + pipeWidth > 70 && (birdY < pipeHeight || birdY + 15 > pipeHeight + pipeGap))) {
    gameOver = true;
    if (score > highScore) {
      highScore = score;
    }
  }
}

void drawUI() {
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TEXT_COLOR);

  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Score: ");
  M5.Lcd.print(score);
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.print("High: ");
  M5.Lcd.print(highScore);
}

void drawBattery() {
  int batteryLevel = M5.Power.getBatteryLevel();
  int batteryWidth = map(batteryLevel, 0, 100, 0, 26);

  M5.Lcd.fillRect(SCREEN_WIDTH - 40, 10, 30, 15, BATTERY_COLOR);
  M5.Lcd.drawRect(SCREEN_WIDTH - 40, 10, 30, 15, WHITE);
  M5.Lcd.fillRect(SCREEN_WIDTH - 10, 15, 5, 5, WHITE);
  M5.Lcd.fillRect(SCREEN_WIDTH - 38, 12, batteryWidth, 11, GREEN);
}

void resetGame() {
  birdY = SCREEN_HEIGHT / 2;
  birdVelocity = 0;
  pipeX = SCREEN_WIDTH;
  score = 0;
  gameOver = false;
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BACKGROUND_COLOR);
  resetGame();
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed() && !gameOver && !gamePaused) {
    birdVelocity = -8;
  }

  if (M5.BtnB.wasPressed()) {
    if (gamePaused) {
      gamePaused = false;
      M5.Lcd.fillScreen(BACKGROUND_COLOR);
    } else {
      gamePaused = true;
      M5.Lcd.fillScreen(BLACK);
    }
  }

  if (!gamePaused && millis() - lastUpdateTime > 50) {
    lastUpdateTime = millis();

    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 10, BACKGROUND_COLOR);
    drawPipes();
    drawBird();
    drawUI();
    drawBattery();
    drawGrass();
    updateGame();

    if (gameOver) {
      M5.Lcd.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("Game Over!");
      delay(2000);
      resetGame();
    }
  }
}
