#ifndef Snake_H
#define Snake_H
#include "Joystick.h"
struct Node {
    int x;
    int y; 
    Node *next;
};

struct Fruit {
    int x;
    int y;
};

class Snake {
public:
    Snake();
    ~Snake();

    int score = -1; // set to -1 so the first fruit spawned sets it to 0

    Node *head;
    Node *tail;
    Fruit *fruit;

    Direction direction; // char to represent direction 'u', 'd', 'l', 'r' { up, down, left, right }

    void respawnFruit();
    void move(Direction d);

};

#endif