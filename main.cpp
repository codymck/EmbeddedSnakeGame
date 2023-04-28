#include "Game.h"
#include "Joystick.h"
#include "max7219.h"
#include "mbed.h"
#include <vector>

Max7219 max7219(D11, D12, D13, D10); // declare pins for 8x8 LED display
Joystick j(A1, A0, D2);              // declare pins for joystick

Game g; // declare game board
Direction dir;

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

void deathScreen() {
  srand(time(NULL));
  vector<int> columns = {0, 0, 0, 0, 0, 0, 0, 0};
  int count = 0;
  while (true) {
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
                                 .intensity = Max7219::MAX7219_INTENSITY_1,
                                 .scan_limit = Max7219::MAX7219_SCAN_8};

  max7219.init_device(cfg);
  max7219.enable_device(1);
  thread_sleep_for(1000);
  max7219.display_all_on();

  // deathScreen();
  // thread_sleep_for(1000);
  // startAnimation();

  thread_sleep_for(1000);

  while (1) {
    Game s;
    g = s;
    while (!j.button_pressed()){ };
    startAnimation();
    g.gameOver = false;

    dir = r;
    //   max7219.display_all_off();
    while (g.gameOver == false) {
      // printf("direction = %i\n", d);
      int sleep = 0;
      while (sleep < sleepTime) {
        if (j.get_direction() != CENTRE) {
          dir = j.get_direction();
        }
        thread_sleep_for(1);
        sleep++;
      }
      g.update();

      g.s->move(dir);

      printf("Score: %d\n", g.s->score);

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
    deathScreen();
  }
}