#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    // Completar (Ejercicio 2)
    // Podriamos pre-crear el hashmap y hacerle free, pero luego eso produce que muchos pedazos de memoria sean creados innecesariamente. 
    // Tradeamos memoria por tiempo de serializacion. Ideas? Habria que medirlo, no se sabe que es mejor.
    unsigned int clave_hash = HashMapConcurrente::hashIndex(clave);

    disponibilidad_por_letra[clave_hash].lock();
    for (auto &p : *tabla[clave_hash]) {
        if(p.first == clave) {
            // p es mutable?
            p.second++;
            disponibilidad_por_letra[clave_hash].unlock();
            return;
        }
    }
    // Si estas significa que no lo encontraste => Hay que insertar

    // posible leakeo de memoria. Cuando hacemos free del pair?
    // se puede hacer, pero no nos matemos con este tema, no se suele evaluar que leakee.
    hashMapPair* nuevaEntrada = new hashMapPair(clave, 1);
    ListaAtomica<hashMapPair>* a = tabla[clave_hash];
    a->insertar(nuevaEntrada);
    disponibilidad_por_letra[clave_hash].unlock();
}

std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
    std::vector<std::string> _claves = std::vector<std::string>();
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            _claves.push_back(p.first);
        }
    }
    return _claves;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
    unsigned int clave_hash = HashMapConcurrente::hashIndex(clave);
    for (auto &p : *tabla[clave_hash]) {
        if(p.first == clave) {
            return p.second;
        }
    }
    return 0;
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
}

#endif
