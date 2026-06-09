#pragma once
#include <iostream>
#include <string>
using namespace std;

template <typename T>
struct NodoQueue {
    T dato;
    NodoQueue<T>* siguiente;
};

template <typename T>
class Queue {
private:
    NodoQueue<T>* frente;
    NodoQueue<T>* fin;
    int tamanio;

public:
    Queue() : frente(nullptr), fin(nullptr), tamanio(0) {}

    void encolar(T dato) {
        NodoQueue<T>* nuevo = new NodoQueue<T>{dato, nullptr};
        if (!fin) {
            frente = fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
        tamanio++;
    }

    bool desencolar(T& resultado) {
        if (!frente) return false;
        resultado = frente->dato;
        NodoQueue<T>* temp = frente;
        frente = frente->siguiente;
        if (!frente) fin = nullptr;
        delete temp;
        tamanio--;
        return true;
    }

    bool estaVacia() { return frente == nullptr; }
    int getTamanio() { return tamanio; }

    NodoQueue<T>* getFrente() { return frente; }

    ~Queue() {
        T temp;
        while (!estaVacia()) desencolar(temp);
    }
};
