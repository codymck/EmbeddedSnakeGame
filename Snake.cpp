#include "Snake.h"
#include <iostream>

Snake::Snake() {
    head = new Node();
    tail = new Node();

    fruit = new Fruit();

    direction = r;

    head->x = 1;
    head->y = 0;

    tail->x = 0;
    tail->y = 0;

    head->next = tail;
    tail->next = nullptr;
}

Snake::~Snake() {

}

bool opposite(Direction d1, Direction d2){
    if((d1 == u && d2 == d) || (d1 == d && d2 == u)){
        return true;
    }else if((d1 == l && d2 == r) || (d1 == r && d2 == l)){
        return true;
    }
    return false;
}

void Snake::respawnFruit(){
    srand(time(NULL));
    Node *n = head;
    int randomX = rand() % 8;
    int randomY = rand() % 8;
    while (n != tail) {
        if(n->x == randomX && n->y == randomY){
            randomX = rand() % 8;
            randomY = rand() % 8;
            n = head;
            continue;
        }
        n = n->next;
    }

    fruit->x = randomX;
    fruit->y = randomY;
    score++;
}

void Snake::move(Direction dir) {
    Node *forward = new Node();
    if(dir != CENTRE && !opposite(direction, dir)){
        direction = dir;
    }
    forward->next = head;
    forward->x = head->x;
    forward->y = head->y;

    if (direction == r) {
        forward->x++;
    }else if (direction == d) {
        forward->y++;
    }else if (direction == l) {
        forward->x--;
    }else if (direction == u) {
        forward->y--;
    }
    head = forward;
    while (forward->next != tail) {
        forward = forward->next;
    }
    // once forward->next = tail
    if(fruit->x != head->x || fruit->y != head->y){ // check if snake is eating fruit
        delete tail;
        tail = forward;
        tail->next = nullptr;
    }else{
        respawnFruit();
    }
}
