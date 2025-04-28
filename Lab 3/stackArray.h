#ifndef STACKARRAY_H
#define STACKARRAY_H
#include <string>
using namespace std;
const int MAX_STACK_SIZE = 100;

class stackArray {
    private:
        int stackTop = 0;
        string stack[MAX_STACK_SIZE];

    public:
        void push(const string& item);
        void pop();
        string top() const;
        bool isEmpty() const;
        bool isFull() const;
};

#endif

