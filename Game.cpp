#include "Game.h"
#include <iostream>

Game::Game() {
    gameOver = false;

    // board[row][column]     board[y][x]
    int board[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    s = new Snake();
    update();
}

Game::~Game() {

}

void Game::update() {
    resetBoard();
    Node *n = s->head;

    while (n != s->tail) {
        board[n->y][n->x] = 1;
        n = n->next;
    }
    board[n->y][n->x] = 1;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          printf("%d", board[i][j]);  
        }
        printf("\n");
    }
}

void Game::resetBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = 0;
        }
    }
}