/*
* asegurarme que el numero de paginas sea el adecuado.
*

Index_page : nodos dentro de la pagina de indices.
Leaf_page: nodos dentro del archivo de hojas.

*/
#ifndef ISAM_NODOS_H
#define ISAM_NODOS_H

#include <iostream>
#include <vector>
#include <tuple>
#include "./others.h"


#define PAGE_SIZE 2000

//DEFINIR M y K 
template<typename TK>
inline static constexpr long M = (PAGE_SIZE - sizeof(int) - sizeof(long)) / (sizeof(Pares<TK>) + sizeof(long));


template<typename TK>
inline static constexpr long N = (PAGE_SIZE - sizeof(int) - sizeof(long)) / (sizeof(Pares<TK>));


//archivo de indices 1,2,3
// pares: tupla<key,posicion>
template<typename Key>
class IndexPage{
    Pares<Key> page[M<Key>];
    long children[M<Key> + 1]; 
    int count;
public:
    IndexPage(){count = 0;}

    bool insert(Pares<Key> par){
        if(count == M<Key>) return false;

        page[count] = par;
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
    Pares<Key> page[N<Key>];
    int count;
    long next;

    DataPage(){count = 0;next = -1;}

    bool insert(Pares<Key> par){
        if(count == N<Key>) return false;

        page[count] = par;
        count++;
        return true;
    }

    long getNext(){return next;}

    //busco entre los hijos y retorno coincidencias.
    std::vector<long> search(Key key){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(page[i].getKey() == key){
                res.push_back(page[i].getPos());
            }
        }

        return res;
    }


    //busco entre los hijos y retorno coincidencias.
    std::vector<long> range(Key inf, Key sup){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(inf <= page[i].getKey() && page[i].getKey() < sup){
                res.push_back(page[i].getPos());
            }
        }

        return res;
    }
};

#endif //ISAM_NODOS_H