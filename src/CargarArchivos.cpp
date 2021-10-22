#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <pthread.h>

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

struct parametrosThread{
    std::atomic<int>* ultimoFileLeido;
    std::vector<std::string> &filePaths;
    HashMapConcurrente &hashMap;
};

void* codigoThread(void* punteroAParametros) {
    
    parametrosThread parametros = *((parametrosThread*) punteroAParametros);

    while(parametros.ultimoFileLeido->fetch_add(1) < parametros.filePaths.size()){
        cargarArchivo(parametros.hashMap,parametros.filePaths[(*parametros.ultimoFileLeido).load()]);
    }
}

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    std::atomic<int> ultimoFileLeido(0);

    parametrosThread parametros {
        .ultimoFileLeido = &ultimoFileLeido,
        .filePaths = filePaths,
        .hashMap = hashMap
    };
    pthread_t threadIds[cantThreads];
    for(unsigned int i = 0; i<cantThreads; i++) {
        pthread_create(&(threadIds[i]), NULL, codigoThread, &parametros);
    }
    for(unsigned int i = 0; i<cantThreads; i++) {
        pthread_join(threadIds[i], NULL);
    }
}



#endif
