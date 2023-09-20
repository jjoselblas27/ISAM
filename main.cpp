#include <iostream>
#include "ISAM.h"

template<typename Record>
int GetKeyFromRecord(const Record& player) {
    // Supongamos que la clave es el nombre del jugador
    return player.value;
}


template<typename TK, typename Record>
void pRead(ISAM<TK,Record> &isam){
    long num;
    cin >> num;

    vector<Player> res = isam.Search(num);
    
    cout << res.size() << endl;

    for(auto item : res){
        cout << item.to_string() << endl;
    }
}


template<typename TK, typename Record>
void pInsert(ISAM<TK,Record> &isam){
    
    Record player("jose", "venezuela", "FCPORCINOS", 28, 28,"LATERAL", 2835, 28,170,68);

    //cout << player.to_string() << endl;
    isam.insertRecord(player);
}

template<typename TK, typename Record>
void pRemove(ISAM<TK,Record> &isam){
    long key;
    cin >> key;

    isam.remove(key);   
}

template<typename TK, typename Record>
void pRange(ISAM<TK,Record> &isam){
    long inf,sup;
    cin >> inf >> sup;

    vector<Record> res = isam.rangeSearch(inf,sup);

    cout << res.size() << endl;

    for(auto item : res){
        cout << item.to_string() << endl;
    }
}


int main() {

    //la key sea value
    ISAM<long, Player> isam("database/Fifa 23 Players Data Short.csv", "datafile", "value",  GetKeyFromRecord<Player>);

    cout << "indexPage: " << sizeof(IndexPage<long>) 
    << " DataPage: " << sizeof(DataPage<long>) << endl;


    //pInsert(isam);
    //pRead(isam);
    //pRemove(isam);
    pRange(isam);
    return 0;
}