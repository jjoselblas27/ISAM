#ifndef OTHERS_H
#define OTHERS_H

#include <iostream>
#include <string>
#include "./record.h"
using namespace std;


bool existe(string name){
    ifstream archivo(name);

    bool res = archivo.good();
    archivo.close();

    return res;
}


template<typename TK>
struct Pares{
    TK key;
    long position;

    Pares(){};
    Pares(TK key, long position){
        this->key = key;
        this->position = position;
    }

    TK getKey(){
        return this->key;
    }

    long getPos(){
        return position;
    }
};


#endif //OTHERS_H