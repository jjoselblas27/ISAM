#include "others.h"
#include   <iostream>
using namespace std;

void prueba(){
    string p = "5.29e-05";

    //double value = string_to_double(p);
    double value = std::stod(p);
    cout << value << endl;
}

int main(){
    prueba();
}