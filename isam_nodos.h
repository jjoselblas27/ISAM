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
//inline static constexpr long M = (PAGE_SIZE - sizeof(int) - sizeof(long)) / (sizeof(Pares<TK>) + sizeof(long));
inline static constexpr long M = 2;

template<typename TK>
//inline static constexpr long N = (PAGE_SIZE - sizeof(int) - sizeof(long)) / (sizeof(Pares<TK>));
inline static constexpr long N = 2;

// pares: tupla<TK,posicion>
template<typename TK>
struct IndexPage{
    Pares<TK> page[M<TK>];
    long children[M<TK> + 1] = {0}; 
    int count;
public:
    IndexPage(){count = 0;}

    bool insertPar(Pares<TK> par){
        if(count == M<TK>) return false;

        page[count] = par;
        count++;
        return true;
    }
    
    void insertChildren(long pos){
        children[count] = pos;
    }

    //busco entre los hijos y retorno coincidencias.
    long search(TK key){
        int i = 0;
        for(; i < count; i++){
            if(page[i].key > key) return children[i];
        }

        return children[i];
    }

    //is full las llaves m
    bool is_full(){
        return count >= M<TK>;
    }

    void reset(){
        this->count = 0;
    }
};



//
//archivo de datos
template<typename TK>
struct DataPage{
    Pares<TK> page[N<TK>];
    int count;
    long next;

    DataPage(){count = 0;next = -1;}

    bool insert(Pares<TK> par){
        if(count == N<TK>) return false;

        page[count] = par;
        count++;
        return true;
    }

    long getNext(){return next;}

    //busco entre los hijos y retorno coincidencias.
    std::vector<long> search(TK key){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(page[i].key == key){
                res.push_back(page[i].position);
            }
        }

        return res;
    }


    //busco entre los hijos y retorno coincidencias.
    std::vector<long> range(TK inf, TK sup){
        std::vector<long> res;
        for(int i = 0; i < count; i++){
            if(inf <= page[i].key && page[i].key < sup){
                res.push_back(page[i].position);
            }
        }
        return res;
    }

    void reset(){
        this->count = 0;
        this->next = -1;
    }

    void show(){
        for (int i=0;i<count;i++){
            cout <<"(" << page[i].key << ", " << page[i].position << "), ";
        }
        cout << ". next: " << next << endl;
    }
};

#endif //ISAM_NODOS_H