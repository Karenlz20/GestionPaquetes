#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int FILAS = 4;
const int COLS  = 6;
const string LIBRE = "------";

class Matriz {
private:
    string mapa[FILAS][COLS];
    int ocupados;

public:
    Matriz() : ocupados(0) {
        for (int i = 0; i < FILAS; i++)
            for (int j = 0; j < COLS; j++)
                mapa[i][j] = LIBRE;
    }

    bool asignar(string idPaquete) {
        if (ocupados >= FILAS * COLS) {
            cout << "  [!] Almacén lleno.\n";
            return false;
        }
        string cod = idPaquete.length() > 6 ? idPaquete.substr(0, 6) : idPaquete;
        for (int i = 0; i < FILAS; i++)
            for (int j = 0; j < COLS; j++)
                if (mapa[i][j] == LIBRE) {
                    mapa[i][j] = cod;
                    ocupados++;
                    return true;
                }
        return false;
    }

    bool liberar(string idPaquete) {
        string cod = idPaquete.length() > 6 ? idPaquete.substr(0, 6) : idPaquete;
        for (int i = 0; i < FILAS; i++)
            for (int j = 0; j < COLS; j++)
                if (mapa[i][j] == cod) {
                    mapa[i][j] = LIBRE;
                    ocupados--;
                    return true;
                }
        return false;
    }

    void mostrar() {
        cout << "\n  ╔══ MAPA DEL ALMACÉN (" << FILAS << "x" << COLS << ") ══╗\n";
        cout << "  Espacios ocupados: " << ocupados
             << " / " << FILAS * COLS << "\n\n";

        cout << "        ";
        for (int j = 0; j < COLS; j++)
            cout << " Col" << j+1 << "   ";
        cout << "\n";

        for (int i = 0; i < FILAS; i++) {
            cout << "  Fila " << i+1 << ":";
            for (int j = 0; j < COLS; j++) {
                if (mapa[i][j] == LIBRE)
                    cout << " [" << LIBRE << "]";
                else
                    cout << " [" << setw(6) << left << mapa[i][j] << "]";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    int getOcupados() { return ocupados; }
    int getCapacidad() { return FILAS * COLS; }
};
