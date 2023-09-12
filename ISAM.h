#ifndef ISAM_H
#define ISAM_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <math.h>
#include <algorithm>
#include "./isam_nodos.h"
#include "./recode.h"

using namespace std;

//Index[2] == indexfile 3
/*
template:
TK -> tipo de dato de la llave
record -> tipo de datos de los registros
RecordTK -> pares<key,position>
getKey -> me retorna el valor de la llave
GREATER -> funcion para comparar Pares<key,position> si es mayor o menor.


ARCHIVOS:
indexFile[i] nombre del archivo de indices de altura i-1
dataFile -> archivo binario con los records
heapFile -> archivo binario con los pares key, posicion
*/

//necesito una funcion a la cual le pase el record y el nombre del atributo y el me retorne la llave.



template<
    typename TK, 
    typename Record = Player,
    typename RecordTK = Pares<TK>,
    typename getKey = std::function<TK(const Record&)>,
    typename Greater = std::function<bool(const Record&, const Record&)>
>
class ISAM{
private:
    fstream File;
    string HeapFile;
    string IndexFile[3];
    string dataFile;

    IndexPage<TK> indiceRAM;

    void buildStruct(vector<Pares<TK>> ord){
        //construir
    }

    //retorna la posicion en el archivo de datos.
    long searchIndice(TK key){
        //busqueda en la altura 2.
        long pos = indiceRAM.search(key);
        //busqueda altura 1
        File.open(IndexFile[1], ios::in|ios::binary)
        File.seekg(pos);
        IndexPage<TK> index;
        File.read((char*) &index, sizeof(index));
        File.close();

        pos = index.search(key);

        //busqueda altura 0
        File.open(IndexFile[0], ios::in|ios::binary)
        File.seekg(pos);
        IndexPage<TK> index;
        File.read((char*) &index, sizeof(index));
        pos = index.search(key);

        File.close();

        return pos;
    }

    void load() {
        std::fstream csvFile(database, std::ios::in);
        File.open(dataFile, ios::out|ios::binary|ios::app);

        long first = -1;
        File.write((char*) &first, sizeof(first));

        std::string line;
        std::getline(csvFile, line);
        while (std::getline(csvFile, line)) {
            std::string knownAs;
            std::string nationality;
            std::string clubName;
            std::string clubJerseyNumberStr;
            std::string overallStr;
            std::string bestPosition;
            std::string valueStr;
            std::string ageStr;
            std::string heightStr;
            std::string weightStr;

            std::stringstream lineStream(line);

            std::getline(lineStream, knownAs, ',');
            std::getline(lineStream, nationality, ',');
            std::getline(lineStream, clubName, ',');
            std::getline(lineStream, clubJerseyNumberStr, ',');
            std::getline(lineStream, overallStr, ',');
            std::getline(lineStream, bestPosition, ',');
            std::getline(lineStream, valueStr, ',');
            std::getline(lineStream, ageStr, ',');
            std::getline(lineStream, heightStr, ',');
            std::getline(lineStream, weightStr, ',');

            int clubJerseyNumber = std::stoi(clubJerseyNumberStr);
            int overall = std::stoi(overallStr);
            int value = std::stoi(valueStr);
            int age = std::stoi(ageStr);
            int height = std::stoi(heightStr);
            int weight = std::stoi(weightStr);

            Record record(knownAs, nationality, clubName, clubJerseyNumber, overall, bestPosition, value, age, height, weight);

            File.write((char*) &record, sizeof(reocord));
            File.write((char*) &first, sizeof(first));
        }

        csvFile.close();
        File.close();
    }

    void buildIsam(){
        File.open(dataFile, ios::in|ios::binary|ios::end);

        long numRecord = File.tellg()/sizeof(Record);
        long minRecord = N*pow(M,3);

        if(numRecord < minRecord){
            //error
            return;
        }

        File.seekg(0,ios::beg);

        std::vector<Pares<TK>> paresKeyPos;
        paresKeyPos.reserve(minRecord);

        Record record;
        for(int i = 0; i < minRecord; i++){
            int pos = File.tellg();
            File.read((char*) &record, sizeof(record));

            Pares<TK> par(getKey(record), pos);
            paresKeyPos.push_back(par);
        }

        //ordenando en RAM
        sort(paresKeyPos.begin(),paresKeyPos.end(), Greater);

        //construyo la estructura del arbol
        buildStruct(paresKeyPos);

        //inserto los que faltan
        long position = File.tellg();
        while(File.read((char*) &record, sizeof(record))){
            Pares<TK> par(getKey(record), position);
            this->insert(par);

            position = File.tellg();
        }

        File.close();

    }




    //manejo una estructura freelist LIFO
    long insertDataFile(Record &record){
        File.open(dataFile, ios::in|ios::out|ios::binary|ios::app);
        //datafile freeelist tipo LIFO
        long header;
        File.seekg(0,ios::beg);
        File.read((char*) &header, sizeof(header));

        //posicion donde escribire.
        long next;
        if(header == -1){
            File.seekp(0,ios::end);
            next = (File.tellp() - sizeof(long)) / (sizeof(Record) + sizeof(long));

            File.write((char*) &record, sizeof(record));
            File.write((char*) &header, sizeof(header)); //header == -1
        }else{
            long empty = -1;
            File.seekg(sizeof(long + header*(sizeof(record) + sizeof(long))) + sizeof(record), ios::beg);
            File.read((char*) &next, sizeof(next));
            
            File.seekp(sizeof(long + header*(sizeof(record) + sizeof(long))), ios::beg);
            File.write((char*) &record, sizeof(record));
            File.write((char*) &empty, sizeof(empty));


            //cambio el header
            File.seekp(0,ios::beg);
            File.write((char*) &next, sizeof(next));
        }
        
        File.close();

        return next;
    }


    //asumo que la posicion es correcta
    void deleteListDataFile(vector<long> positions){
        File.open(dataFile, ios::in|ios::out|ios:binary);

        for(auto pos : positions){
        
            long header;
            File.seekg(0,ios::beg);
            File.read((char*) &header, sizeof(header));

            //sobreescribo en pos
            File.seekp((sizeof(long) + pos*(sizeof(Record) + sizeof(long)) + sizeof(Record)), ios::beg);
            File.write((char*) &header, sizeof(header));

            //sobreescribo en header
            File.seekp(0,ios::beg);
            File.write((char*) &pos, sizeof(pos));

        }

        File.close();
    }
public:
    ISAM(string dataFile, string nameKey){
        this->dataFile = dataFile + ".bin";
        this->HeapFile = "heap_" + nameKey + "_" + datafile + ".bin";
        for(int i=0;i<3;i++){
            this->IndexFile[i] = nameKey + "_indice_" + to_string(i) + ".bin";
        }

        if(!existe(IndexFile[2])){
            buildIsam();
        }

        //cargo a ram el indice
        File.open(IndexFile[2], ios::in|ios::binary);
        File.read((char*) &this->indiceRAM, sizeof(this->indiceRAM));
        File.close();
    }

    //tengo que considerar que pueden haber dataPage vacios.
    void insert(Record record){
        long pos = insertDataFile(record);

        Pares<TK> par(getKey(record), pos);


        //insertar en el arbol
        TK key = par.getKey();
        long position = searchIndice(key);


        File.open(dataFile, ios::in|ios::out|ios::binary);
        DataPage<TK> data;

        next = position;
        do{
            File.seekg(next,ios::beg);
            File.read((char*) &data, sizeof(data));

            if(data.insert(par)){
                //modifico al padre
                File.seekp(next,ios::beg);
                File.write((char*) &data, sizeof(data));
                break;
            }

            next = data.next;

            if(next == -1){
                //llege al ultimo nodo
                DataPage<TK> temp;
                temp.insert(par);

                File.seekp(0,ios::end);
                long nuevo = File.tellp();
                File.write((char*) &temp, sizeof(temp));
                
                //modifico al "padre"
                data.next = nuevo;
                File.seekp(next,ios::beg);
                File.write((char*) &data, sizeof(data));
                
                break;
            }
        }while(true);

        File.close();



    }

    //retorno los records que contenga la misma key
    std::vector<Record> Search(TK key){
        //obtengo la posicion en el archivo de datos(heapfile)
        long position = searchIndice(key);
        DataPage<key> data;
        File.open(HeapFile, ios::in|ios::binary);

        next = position;
        std::vector<long> pos_records;
        do{
            File.seekg(next, ios::beg);
            File.read((char*) &data, sizeof(data));
            
            std::vector<long> temp = data.search(key);

            //insertar al conjunto de resultados
            pos_records.insert(pos_records.end(), temp.begin(), pos_records.end());

            next = data.getNext();
        }while(next != -1);
        File.close();



        //obtengo los datos del archivo de records  real.
        File.open(dataFile, ios::in|ios::binary);
        std::vector<Record> listRecords;

        Record record;
        for (auto pos: pos_records){
            File.seekg(sizeof(long) + pos*(sizeof(Record) + sizeof(long)), ios::beg);
            File.read((char*) &record, sizeof(record));

            listRecords.push_back(record);
        }

        File.close();

        return listRecords;
    }


    /*
    Aprovecho que en la construccion de la estructura tuve un arbol completo, se que hasta cierto limite, k, los dataPages estan ordenados
    */
    std::vector<Record> rangeSearch(Key inf, Key sup){        
        long position = searchIndice(key);
        DataPage data;
        File.open(HeapFile, ios::in|ios::binary);

        std::vector<long> pos_records;

        //MODIFICAR
        long limite = 0;

        bool getnew = false;
        next = position;
        do{        
            //busco en todos los Pares de una pagina y los nexts
            do{
                File.seekg(next, ios::binary);
                File.read((char*) &data, sizeof(data));
                std::vector<long> temp = data.range(inf, sup);
                
                //consegui uno al menos. puede haber mas en estos bloques
                if(!temp.empty()) getnew = true;
                
                //insertar al conjunto de resultados
                pos_records.insert(pos_records.end(), temp.begin(), pos_records.end());

                next = data.getNext();
            }while(next != -1);

            //me muevo a la siguiente pagina.
            next = next + sizeof(DataPage);

        }while(next != limite && getnew);

        File.close();
        
        //obtengo los datos del archivo de records  real.
        File.open(dataFile, ios::in|ios::binary);
        std::vector<Record> listRecords;

        Record record;
        for (auto pos: pos_records){
            File.seekg(sizeof(long) + pos*(sizeof(Record) + sizeof(long)), ios::beg);
            File.read((char*) &record, sizeof(record));

            listRecords.push_back(record);
        }

        File.close();

        return listRecords;
    }


    void remove(TK key){
        long position = searchIndice(key);
        DataPage<key> data;
        File.open(HeapFile, ios::in|ios::binary);

        next = position;
        std::vector<long> pos_records;
        do{
            File.seekg(next, ios::beg);
            File.read((char*) &data, sizeof(data));
            if(count > 0){
                //cambio el ultimo por el que estoy revisando
                for(int i = 0; i < data.count; i++){
                    if(data.page[i] == key){
                        pos_records.push_back(data.page[i].getKey());

                        data.page[i] = data.page[count-1];
                        data.count--;
                    }
                }
            }
            

            next = data.getNext();
        }while(next != -1);
        File.close();

        //elimino en el archivo de records
        deleteListDataFile(pos_records);
    }

};



#endif //ISAM_H