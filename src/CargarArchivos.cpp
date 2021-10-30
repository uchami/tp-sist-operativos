#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

void* codigoThread(std::atomic<int> *ultimoFileLeido, std::vector<std::string> filePaths, HashMapConcurrente *hashMap) {
    int index = (*ultimoFileLeido).fetch_add(1);
    while(index < filePaths.size()){
        cargarArchivo(*hashMap, filePaths[index]);
        index = (*ultimoFileLeido).fetch_add(1);
    }
}

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    std::atomic<int> ultimoFileLeido(0);
    std::thread threads[cantThreads];
    for(unsigned int i = 0; i<cantThreads; i++) {
        threads[i] = std::thread(&codigoThread, &ultimoFileLeido, filePaths, &hashMap);
    }
    for(unsigned int i = 0; i<cantThreads; i++) {
       threads[i].join();
    }
}



#endif
