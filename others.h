#ifndef OTHERS_H
#define OTHERS_H

#include <iostream>
#include <string>
#include <math.h>
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


double string_to_double(string valueStr){
    double value;

    size_t pos = valueStr.find('e');
    if (pos != std::string::npos)
    {
        // Si contiene 'e', separamos la parte antes y después del 'e'
        std::string mantissa = valueStr.substr(0, pos);
        std::string exponent = valueStr.substr(pos + 1);
        // Convierte la mantissa y el exponente a double
        double mantissaValue = std::stod(mantissa);
        int exponentValue = std::stoi(exponent);

        // Calcula el valor final multiplicando por 10 elevado al exponente
        value = mantissaValue * std::pow(10, exponentValue);
    }
    else
    {
        // Si no contiene 'e', simplemente convierte la cadena a double
        value = std::stod(valueStr);
    }

    return value;
}

#endif //OTHERS_H