/*
* asegurarme que el numero de paginas sea el adecuado.
*

Index_page : nodos dentro de la pagina de indices.
Leaf_page: nodos dentro del archivo de hojas.

*/
#ifndef ISAM_NODOS.H
#define ISAM_NODOS.H

#include <iostream>
#include <vector>
#include <tuple>
#include "./others.h"


#define PAGE_SIZE =  2000

//DEFINIR M y K 
long M = 20;
long N = 20;

//archivo de indices 1,2,3
// pares: tupla<key,posicion>
template<typename Key>
class IndexPage{
    Pares<Key> page[M]
    long children[M+1]; 
    int count;
public:
    IndexPage(){count = 0;}

    bool insert(Pares<Key> par){
        if(count == M) return false;

        Page[count] = par;
        count++;
        return true;
    }

    //busco entre los hijos y retorno coincidencias.
    long search(Key key){
        int i = 0;
        for(; i < count; i++){
            if(page[i].getKey() > key) return children[i];
        }

        return children[i];
    }


};



//
//archivo de datos
template<typename Key>
struct DataPage{
    Pares<Key> page[N];
    int count;
    long next;

    IndexPage(){count = 0;next = -1;}

    bool insert(Pares<Key> par){
        if(count == M) return false;

        Page[count] = par;
        count++;
        return true;
    }

    long getNext(){return next;}

    //busco entre los hijos y retorno coincidencias.
    std::vector<long> search(Key key){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(page[i].getKey() == key){
                res.append(page[i].getPos());
            }
        }

        return res;
    }


    //busco entre los hijos y retorno coincidencias.
    std::vector<long> range(Key inf, key sup){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(inf <= page[i].getKey() && page[i].getKey() < sup){
                res.append(page[i].getPos());
            }
        }

        return res;
    }

    void reorder(std::vector<long> pos){

    }


};

#endif //ISAM_NODOS.H