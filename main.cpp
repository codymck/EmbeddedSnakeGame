#include "mbed.h"
#include "max7219.h"
#include "Game.h"

Max7219 max7219(D11, D12, D13, D10);

int conversion(int array[], int len) {
    int output = 0;
    int power = 1;

    for (int i = 0; i < len; i++)
    {
        output += array[(len - 1) - i] * power;
        // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
        power *= 2;
        printf("%d", array[i]);
    }
    printf("\n");
    return output;
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

    Game g;
    for(int i = 1; i < 9; i++){
        unsigned char row = 0;
        for(int j = 0; j < 8; j++){
            int argh = conversion(g.board[i-1], 8);
            max7219.write_digit(1, i, argh);
        }
    }
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


    
    // while(true){
    //     for(int i = 1; i < 9; i+=2){
    //         max7219.write_digit(1, i, 0b10101010);
    //     }
    //     for(int i = 2; i < 9; i+=2){
    //         max7219.write_digit(1, i, 0b01010101);
    //     }
    //     thread_sleep_for(1000);
    //     for(int i = 1; i < 9; i+=2){
    //         max7219.write_digit(1, i, 0b01010101);
    //     }
    //     for(int i = 2; i < 9; i+=2){
    //         max7219.write_digit(1, i, 0b10101010);
    //     }
    //     thread_sleep_for(1000);
    // }



    // uint8_t idx, idy;
    
    // //writes every digit of every device to 0
    // for(idx = 0; idx < 1; idx++){
    //     for(idy = 0; idy < Max7219::MAX7219_DIGIT_7; idy++){
    //         max7219.write_digit((idx + 1), (idy + 1), 0xFF);
    //         thread_sleep_for(1000);
    //     }
    // }

}