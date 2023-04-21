#include "Snake.h"

#ifndef Game_H
#define Game_H

class Game {
private:
        
public:
    Snake *s;
    Game();
    ~Game();

    void update(); // responsible for each cycle of the game and updating the board;

    int board[8][8];
    bool gameOver;
    void checkStatus(); // check if game is over
    void updateFruit();
    void resetBoard();
};

#endif