#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Entidades/Structs.h"
#include "../Estructuras/Lista.h"
using namespace std;

// ─────────────────────────────────────────
//  PERSISTENCIA EN ARCHIVOS .txt
//  Guarda y carga clientes, paquetes
//  y envios usando separador '|'
// ─────────────────────────────────────────

class Archivo {
public:

    // ── GUARDAR ─────────────────────────

    static bool guardarClientes(Lista<Cliente>& lista) {
        ofstream f("clientes.txt");
        if (!f.is_open()) { cout << "  [!] No se pudo abrir clientes.txt\n"; return false; }
        NodoLista<Cliente>* actual = lista.getCabeza();
        while (actual) {
            Cliente& c = actual->dato;
            f << c.id << "|" << c.nombre << "|" << c.telefono << "|" << c.ciudad << "\n";
            actual = actual->siguiente;
        }
        f.close();
        return true;
    }

    static bool guardarPaquetes(Lista<Paquete>& lista) {
        ofstream f("paquetes.txt");
        if (!f.is_open()) { cout << "  [!] No se pudo abrir paquetes.txt\n"; return false; }
        NodoLista<Paquete>* actual = lista.getCabeza();
        while (actual) {
            Paquete& p = actual->dato;
            f << p.id << "|" << p.descripcion << "|" << p.peso << "|" << p.estado << "|" << p.idCliente << "\n";
            actual = actual->siguiente;
        }
        f.close();
        return true;
    }

    static bool guardarEnvios(Lista<Envio>& lista) {
        ofstream f("envios.txt");
        if (!f.is_open()) { cout << "  [!] No se pudo abrir envios.txt\n"; return false; }
        NodoLista<Envio>* actual = lista.getCabeza();
        while (actual) {
            Envio& e = actual->dato;
            f << e.id << "|" << e.idPaquete << "|" << e.ciudadOrigen << "|" << e.ciudadDestino << "|" << e.fecha << "\n";
            actual = actual->siguiente;
        }
        f.close();
        return true;
    }


    static bool cargarClientes(Lista<Cliente>& lista) {
        ifstream f("clientes.txt");
        if (!f.is_open()) return false;
        string linea;
        while (getline(f, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            Cliente c;
            getline(ss, c.id,       '|');
            getline(ss, c.nombre,   '|');
            getline(ss, c.telefono, '|');
            getline(ss, c.ciudad,   '|');
            lista.agregar(c);
        }
        f.close();
        return true;
    }

    static bool cargarPaquetes(Lista<Paquete>& lista) {
        ifstream f("paquetes.txt");
        if (!f.is_open()) return false;
        string linea;
        while (getline(f, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            Paquete p;
            string pesoStr;
            getline(ss, p.id,          '|');
            getline(ss, p.descripcion, '|');
            getline(ss, pesoStr,       '|');
            getline(ss, p.estado,      '|');
            getline(ss, p.idCliente,   '|');
            p.peso = stof(pesoStr);
            lista.agregar(p);
        }
        f.close();
        return true;
    }

    static bool cargarEnvios(Lista<Envio>& lista) {
        ifstream f("envios.txt");
        if (!f.is_open()) return false;
        string linea;
        while (getline(f, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            Envio e;
            getline(ss, e.id,            '|');
            getline(ss, e.idPaquete,     '|');
            getline(ss, e.ciudadOrigen,  '|');
            getline(ss, e.ciudadDestino, '|');
            getline(ss, e.fecha,         '|');
            lista.agregar(e);
        }
        f.close();
        return true;
    }
};
