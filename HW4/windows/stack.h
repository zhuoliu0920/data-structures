#ifndef STACK_H
#define STACK_H

#include <string>

using namespace std;

template <class T>
class Stack
{
private:
    int MAX;
    int toppos;
    T *elements;

public:
    Stack(int size) {MAX = size; toppos = 0; elements = new T[MAX];}
    ~Stack() {delete[] elements;}

    int push(T e) {
        if (isFull()) return -1;
        else {
            elements[toppos++] = e;
            return 0;
        }
    }

    int pop() {
        if (isEmpty()) return -1;
        else {
            toppos--;
            return 0;
        }
    }

    T top() {
        if (isEmpty()) cerr << "Warning: program tries to show top element in an empty stack." << endl;
        return(elements[toppos-1]);
    }

    int isFull() {return (toppos == MAX);}
    int isEmpty() {return (toppos == 0);}
};

#endif
