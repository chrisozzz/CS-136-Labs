#include "stackArray.h"
#include <iostream>

bool stackArray::isEmpty() const{
    return stackTop == 0;
}

bool stackArray::isFull() const{
    return (stackTop == MAX_STACK_SIZE);
}    
    
    
void stackArray::push(const string& item){
    if(!isFull()){
        stack[stackTop]=item;
        stackTop++;
    }else{
        cout << "\nStack is full.\n";
    }
}

void stackArray::pop(){
    if(!isEmpty()){
        stackTop--;
    }else{
        cout << "\nStack is empty.\n";
    }
}

string stackArray::top() const{
    string thing;
    if(!isEmpty()){
        thing = stack[stackTop-1];        
    }else{
        thing = "\0";
    }   
    return thing;
}
