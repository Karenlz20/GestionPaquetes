#pragma once
#include <iostream>
#include <string>
using namespace std;



struct Operacion {
    string tipo;       // INSERT | UPDATE | DELETE | DESPACHO
    string entidad;    // Clientes | Paquetes | Envios
    string id;
    string detalle;
};

struct NodoStack {
    Operacion dato;
    NodoStack* siguiente;
};

class Stack {
private:
    NodoStack* tope;
    int tamanio;

public:
    Stack() : tope(nullptr), tamanio(0) {}

    void push(string tipo, string entidad, string id, string detalle) {
        Operacion op = {tipo, entidad, id, detalle};
        NodoStack* nuevo = new NodoStack{op, tope};
        tope = nuevo;
        tamanio++;
    }

    bool pop(Operacion& resultado) {
        if (!tope) return false;
        resultado = tope->dato;
        NodoStack* temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamanio--;
        return true;
    }

    bool estaVacio() { return tope == nullptr; }
    int getTamanio() { return tamanio; }

    void mostrar() {
        if (!tope) {
            cout << "  [Sin operaciones en el historial]\n";
            return;
        }
        NodoStack* actual = tope;
        int i = 1;
        cout << "  (más reciente → más antiguo)\n\n";
        while (actual) {
            cout << "  [" << i++ << "] "
                 << actual->dato.tipo    << " | "
                 << actual->dato.entidad << " | "
                 << "ID: " << actual->dato.id << " | "
                 << actual->dato.detalle << "\n";
            actual = actual->siguiente;
        }
    }

    ~Stack() {
        Operacion temp;
        while (!estaVacio()) pop(temp);
    }
};
