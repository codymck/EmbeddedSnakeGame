#include "Game.h"
#include "Joystick.h"
#include "max7219.h"
#include "mbed.h"
#include <vector>

Max7219 max7219(D11, D12, D13, D10); // declare pins for 8x8 LED display
Joystick j(A1, A0, D2);              // declare pins for joystick
PwmOut spkr(D3);

Game g; // declare game board
Direction dir;

float volume = 0.075; // increase this to make it louder

int sleepTime = 200; // decrease this to speed it up

int conversion(int array[],
               int len) { // convert array of binary numbers to a decimal number
  int output = 0;
  int power = 1;

  for (int i = 0; i < len; i++) {
    output += array[(len - 1) - i] * power;
    // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
    power *= 2;
    // printf("%d", array[i]);
  }
  // printf("\n");
  return output;
}

void updateScreen() { //
  for (int i = 1; i < 9; i++) {
    unsigned char row = 0;
    for (int j = 0; j < 8; j++) {
      int argh = conversion(g.board[i - 1], 8);
      max7219.write_digit(1, i, argh);
    }
  }
}

void spiralScreen() {
  int offset = 1;
  int y = 4;
  int x = 3;
  float pitch = 0.004;
  g.board[y][x] = 1;
  int direction = 0;
  vector<int> dirs = {0, 1, 2, 3}; // R U L D
  bool flag = false;
  spkr = volume;
  spkr.period(pitch);
  while (x >= 0 && y >= 0) {
    for (int i = 0; i < offset; i++) {
      switch (dirs[direction]) {
      case 0:
        x += 1; // right
        g.board[y][x] = 1;
        thread_sleep_for(100);
        updateScreen();
        break;
      case 1:
        y -= 1; // up
        g.board[y][x] = 1;
        flag = true;
        thread_sleep_for(100);
        updateScreen();
        break;
      case 2:
        x -= 1; // left
        g.board[y][x] = 1;
        thread_sleep_for(100);
        updateScreen();
        break;
      case 3:
        y += 1; // down
        g.board[y][x] = 1;
        flag = true;
        thread_sleep_for(100);
        updateScreen();
        break;
      }
      if (pitch > 0) {
        spkr.period(pitch);
        pitch -= 0.00005;
      }
    }
    if (direction == 4) {
      direction = 0;
    } else {
      direction++;
    }
    if (flag) {
      offset++;
      flag = false;
    }
  }
  spkr = 0;
}

void deathScreen() {
  srand(time(NULL));
  vector<int> columns = {0, 0, 0, 0, 0, 0, 0, 0};
  int count = 0;
  float pitch = 0.0002;
  spkr = volume;
  spkr.period(pitch);
  while (true) {
    if (pitch < 0.004) {
      spkr.period(pitch);
      pitch += 0.0005;
    } else {
      spkr = 0;
    }
    for (int i = 0; i < 8; i++) {
      if ((rand() % 3) + 1 > 1) {
        if (columns.at(i) < 8) {
          g.board[columns.at(i)][i] = 1;
          columns.at(i)++;
          count++;
        }
      }
      thread_sleep_for(25);
    }
    updateScreen();
    if (count >= 64) {
      break;
    }
  }
}

void startAnimation() {
  for (int i = 1; i < 9; i++) {
    max7219.write_digit(1, i, 0b00000000);
    thread_sleep_for(150);
  }
  g.resetBoard();
}

int main() {
  j.init();

  max7219_configuration_t cfg = {.device_number = 1,
                                 .decode_mode = 0,
                                 .intensity = Max7219::MAX7219_INTENSITY_3,
                                 .scan_limit = Max7219::MAX7219_SCAN_8};

  max7219.init_device(cfg);
  max7219.enable_device(1);
  thread_sleep_for(1000);
  max7219.display_all_off();
  g.resetBoard();
  spiralScreen();
  // deathScreen();
  // thread_sleep_for(1000);
  // startAnimation();

  thread_sleep_for(1000);

  while (1) {
    Game s;
    g = s;

    while (!j.button_pressed()) {
    };
    startAnimation();
    g.gameOver = false;
    j.paused = false;

    int score = 0;
    printf("\e[1;1H\e[2J");
    printf("### New Game ###\n");
    printf("Score: 0\n");
    dir = r;
    //   max7219.display_all_off();
    while (g.gameOver == false) {
      while (j.paused == true) {
          thread_sleep_for(1);
      }
      // printf("direction = %i\n", d);
      int sleep = 0;
      while (sleep < sleepTime) {
        if (j.get_direction() != CENTRE) {
          dir = j.get_direction();
        }
        thread_sleep_for(1);
        sleep++;
      }
      spkr = 0;
      g.update();

      g.s->move(dir);

      if (score != g.s->score) {
        score = g.s->score;
        printf("\e[1;1H\e[2J");
        printf("Score: %d\n", score);

        spkr.period(0.001);
        spkr = volume;
      }

      for (int i = 1; i < 9; i++) {
        unsigned char row = 0;
        // for(int j = 0; j < 8; j++){
        //     row = (row << 1) | g.board[i-1][j];
        // }
        int argh = conversion(g.board[i - 1], 8);
        max7219.write_digit(1, i, argh);
      }
    }
    // random button pressed to "reset" it
    j.button_pressed();
    thread_sleep_for(500);
    printf("\e[1;1H\e[2J");
    printf("Game Over\n");
    printf("Final Score: %d\n", score);
    deathScreen();
  }
}