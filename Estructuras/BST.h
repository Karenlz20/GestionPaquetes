#pragma once
#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────
//  ÁRBOL BINARIO DE BÚSQUEDA (BST)
//  Uso: índice de IDs para búsqueda
//  eficiente O(log n)
//  Recorridos: preorden, inorden, postorden
// ─────────────────────────────────────────

struct NodoBST {
    string id;       // ID completo: "PAQ001"
    int    numero;   // Número extraído: 1, 2, 3...
    NodoBST* izquierda;
    NodoBST* derecha;
};

class BST {
private:
    NodoBST* raiz;

    // Extrae el número de un ID (ej: "PAQ003" → 3)
    int extraerNumero(string id) {
        string num = "";
        for (int i = 0; i < (int)id.length(); i++)
            if (id[i] >= '0' && id[i] <= '9')
                num += id[i];
        if (num.empty()) return 0;
        return stoi(num);
    }

    NodoBST* insertar(NodoBST* nodo, string id) {
        if (!nodo) {
            NodoBST* nuevo = new NodoBST();
            nuevo->id        = id;
            nuevo->numero    = extraerNumero(id);
            nuevo->izquierda = nullptr;
            nuevo->derecha   = nullptr;
            return nuevo;
        }
        int num = extraerNumero(id);
        if (num < nodo->numero)
            nodo->izquierda = insertar(nodo->izquierda, id);
        else if (num > nodo->numero)
            nodo->derecha = insertar(nodo->derecha, id);
        return nodo;
    }

    bool buscar(NodoBST* nodo, string id) {
        if (!nodo) return false;
        int num = extraerNumero(id);
        if (nodo->numero == num) return true;
        if (num < nodo->numero) return buscar(nodo->izquierda, id);
        return buscar(nodo->derecha, id);
    }

    NodoBST* minimo(NodoBST* nodo) {
        while (nodo->izquierda) nodo = nodo->izquierda;
        return nodo;
    }

    NodoBST* eliminar(NodoBST* nodo, string id) {
        if (!nodo) return nullptr;
        int num = extraerNumero(id);
        if (num < nodo->numero)
            nodo->izquierda = eliminar(nodo->izquierda, id);
        else if (num > nodo->numero)
            nodo->derecha = eliminar(nodo->derecha, id);
        else {
            if (!nodo->izquierda && !nodo->derecha) { delete nodo; return nullptr; }
            if (!nodo->izquierda) { NodoBST* t = nodo->derecha;   delete nodo; return t; }
            if (!nodo->derecha)   { NodoBST* t = nodo->izquierda; delete nodo; return t; }
            NodoBST* suc = minimo(nodo->derecha);
            nodo->id     = suc->id;
            nodo->numero = suc->numero;
            nodo->derecha = eliminar(nodo->derecha, suc->id);
        }
        return nodo;
    }

    // ── RECORRIDOS ───────────────────────

    // Inorden: izquierda → raíz → derecha (resultado ORDENADO)
    void inorden(NodoBST* nodo) {
        if (!nodo) return;
        inorden(nodo->izquierda);
        cout << "  [" << nodo->numero << "] " << nodo->id << "\n";
        inorden(nodo->derecha);
    }

    // Preorden: raíz → izquierda → derecha
    void preorden(NodoBST* nodo) {
        if (!nodo) return;
        cout << "  [" << nodo->numero << "] " << nodo->id << "\n";
        preorden(nodo->izquierda);
        preorden(nodo->derecha);
    }

    // Postorden: izquierda → derecha → raíz
    void postorden(NodoBST* nodo) {
        if (!nodo) return;
        postorden(nodo->izquierda);
        postorden(nodo->derecha);
        cout << "  [" << nodo->numero << "] " << nodo->id << "\n";
    }

    void visualizar(NodoBST* nodo, string prefijo, bool esDerecha) {
        if (!nodo) return;
        // Primero el subárbol derecho (aparece arriba)
        visualizar(nodo->derecha,   prefijo + (esDerecha ? "        " : "    │   "), true);

        cout << prefijo;
        cout << (esDerecha ? "    ┌── " : "    └── ");
        cout << nodo->id << "\n";

        // Luego el subárbol izquierdo (aparece abajo)
        visualizar(nodo->izquierda, prefijo + (esDerecha ? "    │   " : "        "), false);
    }


    void destruir(NodoBST* nodo) {
        if (!nodo) return;
        destruir(nodo->izquierda);
        destruir(nodo->derecha);
        delete nodo;
    }

public:
    BST() : raiz(nullptr) {}

    void insertar(string id) { raiz = insertar(raiz, id); }
    bool buscar(string id)   { return buscar(raiz, id); }
    void eliminar(string id) { raiz = eliminar(raiz, id); }
    bool estaVacio()         { return raiz == nullptr; }
    NodoBST* getRaiz() { return raiz; }

    bool buscarPorNumero(int num) {
        NodoBST* actual = raiz;
        while (actual) {
            if (actual->numero == num) return true;
            if (num < actual->numero) actual = actual->izquierda;
            else actual = actual->derecha;
        }
        return false;
    }

    // Convierte número a ID completo
    string numeroAId(string prefijo, int num) {
        string n = to_string(num);
        while ((int)n.length() < 3) n = "0" + n;
        return prefijo + n;
    }

    void mostrarInorden() {
        if (!raiz) { cout << "  [Arbol vacio]\n"; return; }
        cout << "  Resultado ORDENADO (izq -> raiz -> der):\n";
        inorden(raiz);
    }

    void mostrarPreorden() {
        if (!raiz) { cout << "  [Arbol vacio]\n"; return; }
        cout << "  Raiz primero (raiz -> izq -> der):\n";
        preorden(raiz);
    }

    void mostrarPostorden() {
        if (!raiz) { cout << "  [Arbol vacio]\n"; return; }
        cout << "  Raiz al final (izq -> der -> raiz):\n";
        postorden(raiz);
    }

    void mostrarArbol() {
        if (!raiz) { cout << "  [Arbol vacio]\n"; return; }

        string etiquetaRaiz = "  [RAIZ] " + raiz->id;
        // El prefijo base tiene el mismo ancho que la etiqueta de la raíz
        string base = string(etiquetaRaiz.length(), ' ');

        // Subárbol derecho arriba
        visualizar(raiz->derecha,   base, true);

        // Raíz en el centro
        cout << etiquetaRaiz << "\n";

        // Subárbol izquierdo abajo
        visualizar(raiz->izquierda, base, false);
    }

    void mostrarTodo() {
        if (!raiz) { cout << "  [Arbol vacio]\n"; return; }

        cout << "\n  ══ Estructura visual del arbol ═════════\n";
        cout << "  Raiz: " << raiz->id << "\n";
        visualizar(raiz->izquierda, "", true);
        visualizar(raiz->derecha,   "", false);

        cout << "\n  ══ Inorden (ordenado ascendente) ═══════\n";
        inorden(raiz);

        cout << "\n  ══ Preorden (raiz primero) ═════════════\n";
        preorden(raiz);

        cout << "\n  ══ Postorden (raiz al final) ═══════════\n";
        postorden(raiz);
    }

    ~BST() { destruir(raiz); }
};
