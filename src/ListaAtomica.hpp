#ifndef LISTA_ATOMICA_HPP
#define LISTA_ATOMICA_HPP

#include <atomic>
#include <cstddef>
#include <mutex>

template<typename T>

class ListaAtomica {
 private:
    struct Nodo {
        Nodo(const T &val) : _valor(val), _siguiente(nullptr) {}

        T _valor;
        Nodo *_siguiente;
    };

    std::atomic<Nodo *> _cabeza;

 public:
    ListaAtomica() : _cabeza(nullptr) {}

    ~ListaAtomica() {
        Nodo *n, *t;
        n = _cabeza.load();
        while (n) {
            t = n;
            n = n->_siguiente;
            delete t;
        }
    }
    std::mutex permiso_para_insertar;


    // Son las dos funciones "inserciones atomicas"?
    // Cual es mejor mutex o compare_exchange? (El compare_exchange hace "busy waiting")
    // weak o strong? que diferencia hay?
    // El weak en algunas arq PUEDE fallar, el strong no. Pero en intel no puede fallar. es un tema de CISC vs RISC
    // Si un proceso se muere con el mutex en la mano se lockea todo.
    // Podriiiamos experimentar pero no es necesario
    // Irse a dormir y despertarse no es gratis, habria que medirlo.
    
    void insertar(const T &valor) {
        // Completar (Ejercicio 1)
        Nodo* newNode = new Nodo(valor);
        newNode->_siguiente = _cabeza;
        
        /* compare_exchange_weak hace esto:
            
        comparacion = (_cabeza.load() == newNode._siguiente);
        if (comparacion) {
            _cabeza = newNode; // soy la nueva cabeza
        } else {
            newNode._siguiente = _cabeza;
        }
        return comparacion;
        */
        while(!(_cabeza.compare_exchange_weak(newNode->_siguiente, newNode)));
        return;
    }

    void insertarConMutex(const T &valor) {
        Nodo* newNode = new Nodo(valor);
        permiso_para_insertar.lock();
        newNode->_siguiente = _cabeza;
        _cabeza = newNode;
        permiso_para_insertar.unlock();
    }

//bool compare_exchange_weak(T& expected, T desired, std::memory_order order);

/*Todas estas funciones toman además de los valores un memory order que se
puede omitir y por defecto es el orden más fuerte (consistencia secuencial).

Notar también que en el caso de los exchanges expected es una referencia, en
el caso de fallar nos devuelve el valor actual de la variable*/

    T& operator[](size_t i) const {
        Nodo *n = _cabeza.load();
        for (size_t j = 0; j < i; j++) {
            n = n->_siguiente;
        }
        return n->_valor;
    }

    unsigned int longitud() const {
        Nodo *n = _cabeza.load();
        unsigned int cant = 0;
        while (n != nullptr) {
            cant++;
            n = n->_siguiente;
        }
        return cant;
    }

    struct iterator {
    private:
        ListaAtomica *_lista;

        typename ListaAtomica::Nodo *_nodo_sig;

        iterator(ListaAtomica<T> *lista, typename ListaAtomica<T>::Nodo *sig)
            : _lista(lista), _nodo_sig(sig) {}

    public:
        iterator &operator=(const typename ListaAtomica::iterator &otro) {
            _lista = otro._lista;
            _nodo_sig = otro._nodo_sig;
            return *this;
        }

        T& operator*() {
            return _nodo_sig->_valor;
        }

        iterator& operator++() { 
            _nodo_sig = _nodo_sig->_siguiente;
            return *this;
        }

        iterator operator++(int) { 
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const typename ListaAtomica::iterator &otro) const {
            return _lista->_cabeza.load() == otro._lista->_cabeza.load()
                && _nodo_sig == otro._nodo_sig;
        }

        bool operator!=(const typename ListaAtomica::iterator &otro) const {
            return !(*this == otro);
        }

        friend iterator ListaAtomica<T>::begin();
        friend iterator ListaAtomica<T>::end();
    };

    iterator begin() { 
        return iterator(this, _cabeza);
    }

    iterator end() { 
        return iterator(this, nullptr);
    }
};

#endif /* LISTA_ATOMICA_HPP */
