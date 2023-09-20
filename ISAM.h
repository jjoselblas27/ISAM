#ifndef ISAM_H
#define ISAM_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <math.h>
#include <algorithm>
#include <stdexcept>
#include "./isam_nodos.h"
#include "./recode.h"

using namespace std;

/*
nivel 1 ->                      root
nivel 2 ->       indexPAGE1        indexPAGE2  ..
nivel 3 -> indexPAge1   indexPAge2   .......

estructura del datafile (freelist LIFO):
            header
            record1     next1
            record2     next2
            record3     next3
            -----

estructura heapfile:
    Page1   Page2   Page3   ---- PageK | Pagek+1  PageK+2 ...
Las paginas desde Page 1 a k estan ordenadas

ARCHIVOS:
indexFile[i] nombre del archivo de indices de nivel i+1
dataFile -> archivo binario con los records
heapFile -> archivo binario semi-ordenado con los pares key, posicion

template:
TK -> tipo de dato de la llave
record -> tipo de datos de los registros
getLlave -> me retorna el valor de la llave

*/
template<
    typename TK, 
    typename Record = Player,
    typename getLlave = std::function<TK(const Record&)>>
class ISAM{
private:
    fstream File;
    string HeapFile;
    string IndexFile[3];
    string dataFile;

    getLlave getKey;
    IndexPage<TK> indiceRAM;

    void buildStruct(vector<Pares<TK>> ord, long numPages){
        File.open(HeapFile, ios::out|ios::binary|ios::app);
        fstream index3(IndexFile[2],ios::out|ios::binary|ios::app);//nivel 3
        fstream index2(IndexFile[1],ios::out|ios::binary|ios::app);//nivel 2
        fstream index1(IndexFile[0],ios::out|ios::binary|ios::app); //root

        long k1 = 0, k2 = 0, k3 = 0;

        DataPage<TK> dataPage;
        IndexPage<TK> indexPage3;
        IndexPage<TK> indexPage2;
        IndexPage<TK> indexPage1;

        cout << "build struct: \n" << endl;

        for (int i = 0; i < numPages; i++){
            DataPage<TK> dataPage;
            for(int j = 0; j < N<TK>; j++){
                dataPage.insert(ord[i*N<TK> + j]);
            }

            for(int i=0;i<N<TK>;i++){
                cout << dataPage.page[i].key <<" - " << dataPage.page[i].position << "   "; 
            }
            cout << endl << endl;

            //escribo en memoria
            File.write((char*) &dataPage, sizeof(dataPage));

            if(i == 0){
                continue;
            }
            //primera key de la pagina.
            Pares<TK> par = dataPage.page[0];
            if(!indexPage3.is_full()){
                indexPage3.insertChildren((k3++) * sizeof(DataPage<TK>));
                indexPage3.insertPar(par);
                
                cout << par.key << " - pos3 "  << ((k3-1) * sizeof(DataPage<TK>)) << endl;

            }else{
                indexPage3.insertChildren((k3++) * sizeof(DataPage<TK>));

                cout << "write3 - " << (k3-1) * sizeof(DataPage<TK>) << endl;
                for(int i=0;i<M<TK>;i++){
                    cout << indexPage3.page[i].key << "   "; 
                }
                cout << endl;

                for(int i=0;i<M<TK>+1;i++){
                    cout << indexPage3.children[i] << "   "; 
                }
                cout << endl << endl;

                index3.write((char*) &indexPage3, sizeof(indexPage3));
                indexPage3.reset();

                if(!indexPage2.is_full()){
                    indexPage2.insertChildren((k2++) * sizeof(IndexPage<TK>));
                    indexPage2.insertPar(par);
                    
                    cout <<  par.key << " pos2 - " << (k2-1) * sizeof(IndexPage<TK>) << endl;
                }else{
                    indexPage2.insertChildren((k2++) * sizeof(IndexPage<TK>));
                    cout << "write2 - " << (k2-1) * sizeof(IndexPage<TK>) << endl;

                    cout << "write3 - " << (k3-1) * sizeof(DataPage<TK>) << endl;
                    for(int i=0;i<M<TK>;i++){
                        cout << indexPage2.page[i].key << "   "; 
                    }
                    cout << endl;

                    for(int i=0;i<M<TK>+1;i++){
                        cout << indexPage2.children[i] << "   "; 
                    }
                    cout << endl << endl;

                    index2.write((char*) &indexPage2, sizeof(IndexPage<TK>));

                    indexPage2.reset();

                    //escribir en el root.
                    indexPage1.insertChildren((k1++) * sizeof(IndexPage<TK>));
                    indexPage1.insertPar(par);
                    cout <<  par.key << "pos1 - " << (k1-1) * sizeof(IndexPage<TK>) << endl;
                }
            }
        }


        indexPage3.insertChildren(k3 * sizeof(DataPage<TK>));
        index3.write((char*) &indexPage3, sizeof(IndexPage<TK>));


        cout << "write3 - " << (k3) * sizeof(DataPage<TK>) << endl;
        for(int i=0;i<M<TK>;i++){
            cout << indexPage3.page[i].key << "   "; 
        }
        cout << endl;

        for(int i=0;i<M<TK>+1;i++){
            cout << indexPage3.children[i] << "   "; 
        }
        cout << endl << endl;

        indexPage2.insertChildren(k2 * sizeof(IndexPage<TK>));
        index2.write((char*) &indexPage2, sizeof(IndexPage<TK>));

        cout << "write2 - " << (k2) * sizeof(DataPage<TK>) << endl;
        for(int i=0;i<M<TK>;i++){
            cout << indexPage2.page[i].key << "   "; 
        }
        cout << endl;

        for(int i=0;i<M<TK>+1;i++){
            cout << indexPage2.children[i] << "   "; 
        }
        cout << endl << endl;

        indexPage1.insertChildren(k1 * sizeof(IndexPage<TK>));
        index1.write((char*) &indexPage1, sizeof(IndexPage<TK>));

        cout << "write1 - " << (k1) * sizeof(DataPage<TK>) << endl;
        for(int i=0;i<M<TK>;i++){
            cout << indexPage1.page[i].key << "   "; 
        }
        cout << endl;

        for(int i=0;i<M<TK>+1;i++){
            cout << indexPage1.children[i] << "   "; 
        }
        cout << endl << endl;

        File.close();
        index3.close();
        index2.close();
        index1.close();
    }
    
    //retorna la posicion de la pagina en el heapfile
    long searchIndice(TK key){

        cout << "\n\n searchIndex " << endl;

        IndexPage<TK> index2;
        IndexPage<TK> index3;
        //busqueda en el nivel 1
        long pos = indiceRAM.search(key);
        cout << "nivel 1 - " << pos << endl;
        //busqueda en nivel 2
        File.open(IndexFile[1], ios::in|ios::binary);
        File.seekg(pos, ios::beg);
        File.read((char*) &index2, sizeof(index2));
        File.close();

        pos = index2.search(key);
        cout << "nivel 2 - " << pos << endl; 

        //busqueda en nivel 3
        File.open(IndexFile[2], ios::in|ios::binary);
        File.seekg(0,ios::end);

        cout << "pos max3 : " << File.tellg() << endl;

        File.seekg(pos, ios::beg);
        File.read((char*) &index3, sizeof(index3));
        File.close();

        pos = index3.search(key);
        cout << "nivel 3 - " << pos << endl;
        return pos;
    }

    void load(string database) {
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

            File.write((char*) &record, sizeof(record));
            File.write((char*) &first, sizeof(first));
        }

        csvFile.close();
        File.close();
    }

    void buildIsam(){
        //datafile contine los records.
        fstream FileData(dataFile, ios::in|ios::binary);
        FileData.seekg(0, ios::end);
        
        long numRecord = (FileData.tellg() - sizeof(long)) / (sizeof(Record) + sizeof(long));
        long minRecord = N<TK>*pow(M<TK>+1,3);

        if(numRecord < minRecord){
            std::cerr << "La cantidad de records es insuficiente" << std::endl;
            exit(1); // 1 indica un error, puedes usar otro código de salida si lo prefieres
        }

        std::vector<Pares<TK>> paresKeyPos;
        paresKeyPos.reserve(minRecord);

        cout << "pares:" << endl;

        //me ubico despues del header.
        FileData.seekg(sizeof(long),ios::beg);
        long basura;
        Record record;
        for(int i = 0; i < minRecord; i++){
            int pos = (FileData.tellg() - sizeof(long)) / (sizeof(Record) + sizeof(long));
            FileData.read((char*) &record, sizeof(record));
            FileData.read((char*) &basura, sizeof(basura));

            Pares<TK> par(getKey(record), pos);
            paresKeyPos.push_back(par);
        }

        //ordenando en RAM
        sort(paresKeyPos.begin(),paresKeyPos.end(), [](const Pares<TK>& a, const Pares<TK>& b) {
        return a.key < b.key;
        });

        //PRUEBAS
        for(auto par : paresKeyPos){
            cout << par.key << ", " << par.position << endl;
        }

        //construyo la estructura del arbol
        buildStruct(paresKeyPos, pow(M<TK>+1,3));


        //cout << "despues struct:" <<  endl;
        //inserto los que faltan
        long position = (FileData.tellg() - sizeof(long)) /(sizeof(Record) + sizeof(long)) ;
        while(FileData.read((char*) &record, sizeof(record))){
            FileData.read((char*) &basura, sizeof(basura));
            Pares<TK> par(getKey(record), position);
            this->insert(par);

            //cout << par.key << ", " << par.position << endl;

            position = (FileData.tellg() - sizeof(long)) /(sizeof(Record) + sizeof(long)) ;
        }


        FileData.close();
    }


    //manejo una estructura freelist LIFO, retorna una posicion relativa en el datafile
    long insertDataFile(Record &record){
        File.open(dataFile, ios::in|ios::out|ios::binary|ios::app);
        //datafile freeelist tipo LIFO
        long header;
        File.seekg(0,ios::beg);
        File.read((char*) &header, sizeof(header));

        //posicion donde escribire.
        long position;
        if(header == -1){
            File.seekp(0,ios::end);
            position = (File.tellp() - sizeof(long)) / (sizeof(Record) + sizeof(long));

            File.write((char*) &record, sizeof(record));
            File.write((char*) &header, sizeof(header)); //header == -1
        }else{
            long empty = -1;
            long next;

            //transformo header a posicion absoluta
            position = sizeof(long)+ header*(sizeof(record) + sizeof(long)) + sizeof(record);
            File.seekg(position, ios::beg);
            File.read((char*) &next, sizeof(next));
            
            position = sizeof(long) + header*(sizeof(record) + sizeof(long));
            File.seekp(position, ios::beg);
            File.write((char*) &record, sizeof(record));
            File.write((char*) &empty, sizeof(empty));


            //cambio el header
            File.seekp(0,ios::beg);
            File.write((char*) &next, sizeof(next));
        }
        
        File.close();

        return position;
    }


    //tengo un vector con posiciones relativas. freelist LIFO
    void deleteListDataFile(vector<long> positions){
        File.open(dataFile, ios::in|ios::out|ios::binary);

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
    ISAM(string database, string dataFile, string nameKey, getLlave getKey){
        cout << "entre " << endl;
        this->dataFile = "./database/" + dataFile + ".bin";
        this->HeapFile = "./database/heap_" + nameKey + ".bin";
        for(int i=0;i<3;i++){
            this->IndexFile[i] = "./database/" + nameKey + "_indice_" + to_string(i) + ".bin";
        }

        this->getKey = getKey;

        //verifico si existen los indices.
        if(!existe(IndexFile[0])){
            cout << "no existe " << endl;
            //verificar si existe el dataFile
            if(!existe(dataFile)) {
                cout << "load" << endl;
                load(database);

            }
            //return;
            buildIsam();
        }

        cout << "aca" << endl;

        //cargo a ram el root
        File.open(IndexFile[0], ios::in|ios::binary);
        File.read((char*) &this->indiceRAM, sizeof(this->indiceRAM));
        cout << "carge root a RAM" << endl;
        File.close();
    }


    void insertRecord(Record record){
        long pos = insertDataFile(record);
        cout << "insert key: " <<  getKey(record) << " ";
        cout << "pos:" << pos << endl;
        Pares<TK> par(getKey(record), pos);


        this->insert(par);
    }
    //tengo que considerar que pueden haber dataPage vacios.
    void insert(Pares<TK> par){
        cout << "insert" << endl;
        //insertar en el heapfile

        //busco la pagina que me coreesponde.
        long position = searchIndice(par.key);


        fstream fileHeap(HeapFile, ios::in|ios::out|ios::binary);
        DataPage<TK> data;

        long next;
        //valores inicales
        data.next = position;
        do{
            next = data.next;
            fileHeap.seekg(next,ios::beg);
            fileHeap.read((char*) &data, sizeof(data));

            if(data.insert(par)){
                //modifico al padre
                fileHeap.seekp(next,ios::beg);
                fileHeap.write((char*) &data, sizeof(data));

                fileHeap.close();
                //termine de insertar.

                cout << "dentro " << endl;

                return;
            }

        }while(data.next != -1);

        //todos los nodos estaban llenos. creo una nueva pagina
        DataPage<TK> temp;
        temp.insert(par);

        fileHeap.seekp(0,ios::end);
        long nuevo = fileHeap.tellp();
        fileHeap.write((char*) &temp, sizeof(temp));
        
        //modifico al "padre"
        data.next = nuevo;
        fileHeap.seekp(next,ios::beg);
        fileHeap.write((char*) &data, sizeof(data));


        cout << "nuevo: " << nuevo << " padre: " << next << endl;  

        temp.show();

        fileHeap.close();
    }

    //retorno los records que contenga la misma key
    std::vector<Record> Search(TK key){
        cout << "entre al search " << endl;
        //obtengo la posicion en el archivo de datos(heapfile)
        long position = searchIndice(key);
        DataPage<TK> data;
        fstream fileHeap(HeapFile, ios::in|ios::binary);

        long next = position;
        std::vector<long> pos_records;

        cout << "search while, pos:" << next << endl;
        do{
            fileHeap.seekg(next, ios::beg);
            fileHeap.read((char*) &data, sizeof(data));
            
            cout << "count: " << data.count << endl;

            //imprimo el data:
            data.show();

            std::vector<long> temp = data.search(key);
            //std::vector<long> temp;
            //insertar al conjunto de resultados
            pos_records.insert(pos_records.end(), temp.begin(), temp.end());

            if(next == data.next){
                cout << "igual" << endl;
                break;
            }
            next = data.next;
            cout << next << endl;
            
        }while(next != -1);
        fileHeap.close();


        //obtengo los datos del archivo de records  real.
        fstream fileData(dataFile, ios::in|ios::binary);
        std::vector<Record> listRecords;

        Record record;
        for (auto pos: pos_records){
            fileData.seekg(sizeof(long) + pos*(sizeof(Record) + sizeof(long)), ios::beg);
            fileData.read((char*) &record, sizeof(record));

            listRecords.push_back(record);
        }

        fileData.close();

        return listRecords;
    }


    /*
    Aprovecho que en la construccion de la estructura tuve un arbol completo, se que hasta cierto limite, k, los dataPages estan ordenados
    */
    std::vector<Record> rangeSearch(TK inf, TK sup){        
        long infPos = searchIndice(inf);
        long supPos =searchIndice(sup);

        DataPage<TK> data;
        File.open(HeapFile, ios::in|ios::binary);

        std::vector<long> pos_records;

        //MODIFICAR
        long limite = pow(M<TK>, 3);

        long salto = infPos;
        do{        
            //busco en todos los Pares de una pagina y los nexts
            long next = salto;
            do{
                File.seekg(next, ios::beg);
                File.read((char*) &data, sizeof(data));
                std::vector<long> temp = data.range(inf, sup);
                
                //insertar al conjunto de resultados
                pos_records.insert(pos_records.end(), temp.begin(), pos_records.end());

                next = data.getNext();
            }while(next != -1);

            //me muevo a la siguiente pagina.
            salto = salto + sizeof(DataPage<TK>);

        }while(salto <= supPos);

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

        DataPage<TK> data;
        File.open(HeapFile, ios::out|ios::in|ios::binary);

        long next = position;
        std::vector<long> pos_records;
        do{
            File.seekg(next, ios::beg);
            File.read((char*) &data, sizeof(data));
            if(data.count > 0){
                //cambio el ultimo por el que estoy revisando
                for(int i = 0; i < data.count; i++){
                    if(data.page[i].key == key){
                        pos_records.push_back(data.page[i].key);

                        data.page[i] = data.page[data.count-1];
                        data.count--;
                    }
                }
                
                //escribo en memoria el cambio
                File.seekp(next, ios::beg);
                File.write((char*) &data, sizeof(data));
            }

            next = data.next;
        }while(next != -1);
        File.close();

        //elimino en el archivo de records
        deleteListDataFile(pos_records);
    }

};



#endif //ISAM_H