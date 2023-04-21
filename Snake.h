#ifndef Snake_H
#define Snake_H

struct Node {
    int x;
    int y; 
    Node *next;
};

class Snake {
public:
    Snake();
    ~Snake();

    Node *head;
    Node *tail;

    char direction; // char to represent direction 'u', 'd', 'l', 'r' { up, down, left, right }

    void grow();
    void move(char d);

};

#endif