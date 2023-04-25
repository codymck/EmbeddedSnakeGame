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

    Node *head;
    Node *tail;
    Fruit *fruit;

    Direction direction; // char to represent direction 'u', 'd', 'l', 'r' { up, down, left, right }

    void respawnFruit();
    void grow();
    void move(Direction d);

};

#endif