#ifndef HMC_HPP
#define HMC_HPP

#include <mutex>
#include <array>
#include <atomic>
#include <string>
#include <vector>

#include "ListaAtomica.hpp"

typedef std::pair<std::string, unsigned int> hashMapPair;

class HashMapConcurrente {
 public:
    static constexpr int cantLetras = 26;
    //Variables compartidas
    // No esta muy bueno que sean publicas, no? Porque no podemos paralelizar el trabajo de calculo de 'varios maximos al mismo tiempo'.
    std::atomic<int>* indexLetraSinCalcular;
    std::mutex afectarMaximo;
    hashMapPair *max;

    HashMapConcurrente();

    void incrementar(std::string clave);
    std::vector<std::string> claves();
    unsigned int valor(std::string clave);

    hashMapPair maximo();
    hashMapPair maximoParalelo(unsigned int cantThreads);

   //un mutex por cada 
   std::mutex disponibilidad_por_letra[HashMapConcurrente::cantLetras];
   void apagarDisponibilidad();
   void prenderDisponibilidad();
   void* codigoThreadMaximo();

 private:
    ListaAtomica<hashMapPair> *tabla[HashMapConcurrente::cantLetras];

    static unsigned int hashIndex(std::string clave);
};

#endif  /* HMC_HPP */
