#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct NodoLista {
    T dato;
    NodoLista<T>* siguiente;
};

template <typename T>
class Lista {
private:
    NodoLista<T>* cabeza;
    int tamanio;

public:
    Lista() : cabeza(nullptr), tamanio(0) {}

    void agregar(T dato) {
        NodoLista<T>* nuevo = new NodoLista<T>{dato, cabeza};
        cabeza = nuevo;
        tamanio++;
    }

    bool eliminar(string id) {
        if (!cabeza) return false;

        if (cabeza->dato.id == id) {
            NodoLista<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            tamanio--;
            return true;
        }

        NodoLista<T>* actual = cabeza;
        while (actual->siguiente) {
            if (actual->siguiente->dato.id == id) {
                NodoLista<T>* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                tamanio--;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    T* buscar(string id) {
        NodoLista<T>* actual = cabeza;
        while (actual) {
            if (actual->dato.id == id) return &actual->dato;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    bool existe(string id) {
        return buscar(id) != nullptr;
    }

    bool estaVacia() { return cabeza == nullptr; }
    int getTamanio() { return tamanio; }
    NodoLista<T>* getCabeza() { return cabeza; }

    ~Lista() {
        NodoLista<T>* actual = cabeza;
        while (actual) {
            NodoLista<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};
