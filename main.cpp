#include "mbed.h"
#include "max7219.h"
#include "Game.h"
#include <vector>

Max7219 max7219(D11, D12, D13, D10);

Game g;

int conversion(int array[], int len) {
    int output = 0;
    int power = 1;

    for (int i = 0; i < len; i++){
        output += array[(len - 1) - i] * power;
        // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
        power *= 2;
        printf("%d", array[i]);
    }
    printf("\n");
    return output;
}

void updateScreen(){
    for(int i = 1; i < 9; i++){
        unsigned char row = 0;
        for(int j = 0; j < 8; j++){
            int argh = conversion(g.board[i-1], 8);
            max7219.write_digit(1, i, argh);
        }
    }
}

void deathScreen(){
    srand(time(NULL));
    vector<int> columns = {0, 0, 0, 0, 0, 0, 0, 0};
    int count = 0;
    while(true){
        for(int i = 0; i < 8; i++){
            if((rand() % 3) + 1 > 1){
                if(columns.at(i) < 8){
                    g.board[columns.at(i)][i] = 1;
                    columns.at(i)++;
                    count++;
                }
            }
            
        }
        updateScreen();
        if(count >= 64){
            break;
        }
    }
}

void startAnimation(){
    for(int i = 1; i < 9; i++){
        max7219.write_digit(1, i, 0b00000000);
        thread_sleep_for(200);
    }
    g.resetBoard();
}

int main(){
    max7219_configuration_t cfg = {
        .device_number = 1,
        .decode_mode = 0,
        .intensity = Max7219::MAX7219_INTENSITY_1,
        .scan_limit = Max7219::MAX7219_SCAN_8
    };

    max7219.init_device(cfg);
    max7219.enable_device(1);
    thread_sleep_for(1000);
    max7219.display_all_off();

    // show mommy najjar this
    deathScreen();
    thread_sleep_for(1000);
    startAnimation();

    thread_sleep_for(1000);
    int count = 0;

    while (count < 25) {
        g.update();
        if (count < 5) {
            g.s->move('r');
            if(count == 4){
                g.s->grow();
            }
        }
        else if (count < 12) {
            g.s->move('d');
        }
        else if (count < 19){
            g.s->move('l');
        }
        else {
            g.s->move('u');
            if(count == 19){
                g.s->grow();
            }
            else if (count == 21) {
                g.s->grow();
            }
        }
        for(int i = 1; i < 9; i++){
            printf("LOOP \n");
            unsigned char row = 0;
            // for(int j = 0; j < 8; j++){
            //     row = (row << 1) | g.board[i-1][j];
            // }
            int argh = conversion(g.board[i-1], 8);
            printf("%d\n", argh);
            max7219.write_digit(1, i, argh);
        }
        count++;
    }
}