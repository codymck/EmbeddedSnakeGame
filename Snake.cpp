#include "Snake.h"
#include <iostream>

Snake::Snake() {
    head = new Node();
    tail = new Node();

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

void Snake::grow() {
    Node *forward = new Node();
    forward->next = head;
    forward->x = head->x;
    forward->y = head->y;

    if (direction == r) {
        forward->x++;
        head = forward;
    }
    else if (direction == d) {
        forward->y++;
        head = forward;
    }
    else if (direction == l) {
        forward->x--;
        head = forward;
    }
    else if (direction == u) {
        forward->y--;
        head = forward;
    }
}

void Snake::move(Direction dir) {
    Node *forward = new Node();
    if(dir != CENTRE){
        direction = dir;
    }
    forward->next = head;
    forward->x = head->x;
    forward->y = head->y;

    if (direction == r) {
        forward->x++;
        head = forward;
        
        while (forward->next != tail) {
            forward = forward->next;
        }
    }
    else if (direction == d) {
        forward->y++;
        head = forward;
        
        while (forward->next != tail) {
            forward = forward->next;
        }
    }
    else if (direction == l) {
        forward->x--;
        head = forward;
        
        while (forward->next != tail) {
            forward = forward->next;
        }
    }
    else if (direction == u) {
        forward->y--;
        head = forward;
        
        while (forward->next != tail) {
            forward = forward->next;
        }
    }
    // once forward->next = tail
    delete tail;
    tail = forward;
    tail->next = nullptr;

}
