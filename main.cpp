#include "Game.h"
#include "Joystick.h"
#include "max7219.h"
#include "mbed.h"
#include <vector>

Max7219 max7219(D11, D12, D13, D10); // declare pins for 8x8 LED display
Joystick j(A1, A0, PA_10); // declare pins for joystick

Game g; // declare game board

int conversion(int array[], int len) { // convert array of binary numbers to a decimal number
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
    thread_sleep_for(200);
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
  max7219.display_all_off();

   deathScreen();
  // thread_sleep_for(1000);
  // startAnimation();

  thread_sleep_for(1000);
  int count = 0;
  while (1) {
    if (j.button_pressed()) {
        startAnimation();
        g.gameOver = false;
    //   max7219.display_all_off();
      while (g.gameOver == false) {
        Direction d = j.get_direction();
        // printf("direction = %i\n", d);
        g.update();
        if (g.gameOver == true) {
          printf("game ova");
          break;
        }
        g.s->move(d);
        for (int i = 1; i < 9; i++) {
          unsigned char row = 0;
          // for(int j = 0; j < 8; j++){
          //     row = (row << 1) | g.board[i-1][j];
          // }
          int argh = conversion(g.board[i - 1], 8);
          max7219.write_digit(1, i, argh);
        }
        count++;

        if (count % 10 == 0) {
          // g.s->grow();
        }
        thread_sleep_for(200);
     }

      thread_sleep_for(500);
      deathScreen();
      g.gameOver = false;
    }
  }
  // /*while (count < 25) {
  //     g.update();
  //     if (count < 5) {
  //         g.s->move('r');
  //         if(count == 4){
  //             g.s->grow();
  //         }
  //     }
  //     else if (count < 12) {
  //         g.s->move('d');
  //     }
  //     else if (count < 19){
  //         g.s->move('l');
  //     }
  //     else {
  //         g.s->move('u');
  //         if(count == 19){
  //             g.s->grow();
  //         }
  //         else if (count == 21) {
  //             g.s->grow();
  //         }
  //     }*/
  //     for(int i = 1; i < 9; i++){
  //         printf("LOOP \n");
  //         unsigned char row = 0;
  //         // for(int j = 0; j < 8; j++){
  //         //     row = (row << 1) | g.board[i-1][j];
  //         // }
  //         int argh = conversion(g.board[i-1], 8);
  //         printf("%d\n", argh);
  //         max7219.write_digit(1, i, argh);
  //     }
  //     count++;
  // }
}