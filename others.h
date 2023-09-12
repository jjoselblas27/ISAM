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


template<typename Key>
class Pares{
    Key key;
    long position;
public:
    Pares(Key key, long position){
        this->key = key;
        this->position = position;
    }

    Key getKey(){
        return key;
    }

    long getPos(){
        return position;
    }


};


#endif //OTHERS_H