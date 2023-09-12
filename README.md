# ISAM
implementacion de ISAM


## PASOS
1. crear las clases de los nodos(index, page)
2. crear el archivo datafile -> talvez puede ser externo
3. crear ISAM
4. testear




## CLASES NODOS
- IndexPage\<typename key>: guardara un array de M keys, M+1 childrens y su count
- DataPage\<typename Key>: guardara N records, puntero al next y su count


## datafile
- guardara datapages con las keys y su posicion. 
- notar que estara ordenada hasta cierta posicion l. ya que despues de l estaran las paginas que se agregaran debido al desbordamiento.

## ISAM
digamos que K es la cantidad de "records" en las hojas.
- creacion del arbol. Considerare que mi heapfile tiene cuanto menos el numero de registros minimos en las hojas, esto para evitar complicaciones(despues se puede mejorar).
    - cojo los primeros k elementos y los cargo a la ram los pares key, posicion y lo ordeno. 
    - con esos pares ordenados construyo las pages y lo inserto en mi data_file(mi nivel de pages), mientras puedo construir los indices para los datos de arriba(acordarse de el video)
    - Despues utilizo la funcion tipica insert para insertar los demas ya que ya tendre la estructura del arbol hecho.

- search: busco en los indices -> me retorna una posicion en el data_file hago una buscada iterativa entre los pages y los next, si lo encuentro retorno las posiciones en el heap_file.
- insert: hago una busqueda en el arbol, encuentro la hoja y compruebo si puedo insertar, sino me voy al next.(tal vez se puede implementar el desbordamiento con una estructura tipo stack.)
- delete: no modifica los indices: hago una busqueda y elimino los nodos que cumplan con ese criterio. notar que puede quedarme un nodo vacio(no hay problema porque el count sera 0) si dentro de una datapage me queda un espacio vacio en medio puedo mover los datos.
- range: dentro del heap file, se que los datapage\<pares key, value> estan ordeandos hasta una posicion l. entonces selecciono en borde menor y lo busco -> data_page k, realizo una busqueda dentro de la hoja hasta encontrar los valores mayores al minimo y lo guardo, ahora continuo en la data_page k+1 y realizo lo mismo. me detengo cuando ya llege al limite o cuando ya no encuentre mayores en una busqueda en el bloque(y sus nexts).



## NOTAS
- definicion de archivos:
    - database -> archivo csv con los datos.
    - dataFIle -> archivo binario con los records(no importa el orden) y el next para cada uno.
    - heapFIle -> archivo binario con las DataPage, los cuales contienen Pares\<key, position>.
    - IndexFile[i] -> archivo binario con los indices de altura i-1



- estructura del archivo heap_file, trabajare con un free list tipo LIFO. en el next guardare la posicion relativa.(0,1,2,3 ...)

next 

record next

record next 

..
 ...

record next



