#pragma once
#include <iostream>
#include <string>
using namespace std;


const int MAX_CIUDADES = 10;
const int INF = 99999;

class Grafo {
private:
    string ciudades[MAX_CIUDADES];
    int    matriz[MAX_CIUDADES][MAX_CIUDADES];
    int    total;

    int indiceDe(string nombre) {
        for (int i = 0; i < total; i++)
            if (ciudades[i] == nombre) return i;
        return -1;
    }

public:
    Grafo() : total(0) {
        for (int i = 0; i < MAX_CIUDADES; i++)
            for (int j = 0; j < MAX_CIUDADES; j++)
                matriz[i][j] = (i == j) ? 0 : INF;
    }

    bool agregarCiudad(string nombre) {
        if (total >= MAX_CIUDADES) {
            cout << "  [!] Límite de ciudades alcanzado.\n";
            return false;
        }
        if (indiceDe(nombre) != -1) {
            cout << "  [!] La ciudad '" << nombre << "' ya existe.\n";
            return false;
        }
        ciudades[total++] = nombre;
        return true;
    }

    bool agregarRuta(string origen, string destino, int km) {
        if (km <= 0) {
            cout << "  [!] La distancia debe ser mayor a 0.\n";
            return false;
        }
        int a = indiceDe(origen);
        int b = indiceDe(destino);
        if (a == -1 || b == -1) {
            cout << "  [!] Ciudad no encontrada.\n";
            return false;
        }
        matriz[a][b] = matriz[b][a] = km;
        return true;
    }

    void dijkstra(int origen, int destino) {
        if (origen < 0 || origen >= total || destino < 0 || destino >= total) {
            cout << "  [!] Índice de ciudad inválido.\n";
            return;
        }
        if (origen == destino) {
            cout << "  [!] El origen y destino son la misma ciudad.\n";
            return;
        }

        int  dist[MAX_CIUDADES];
        bool visitado[MAX_CIUDADES];
        int  prev[MAX_CIUDADES];

        for (int i = 0; i < total; i++) {
            dist[i]     = INF;
            visitado[i] = false;
            prev[i]     = -1;
        }
        dist[origen] = 0;

        for (int iter = 0; iter < total; iter++) {
            int u = -1;
            for (int i = 0; i < total; i++)
                if (!visitado[i] && (u == -1 || dist[i] < dist[u]))
                    u = i;

            if (u == -1 || dist[u] == INF) break;
            visitado[u] = true;

            for (int v = 0; v < total; v++) {
                if (matriz[u][v] != INF && !visitado[v]) {
                    int nueva = dist[u] + matriz[u][v];
                    if (nueva < dist[v]) {
                        dist[v] = nueva;
                        prev[v] = u;
                    }
                }
            }
        }

        if (dist[destino] == INF) {
            cout << "  [!] No existe ruta entre "
                 << ciudades[origen] << " y " << ciudades[destino] << ".\n";
            return;
        }

        string camino = ciudades[destino];
        int actual = prev[destino];
        while (actual != -1) {
            camino = ciudades[actual] + " → " + camino;
            actual = prev[actual];
        }

        cout << "\n  Ruta más corta:\n";
        cout << "  " << camino << "\n";
        cout << "  Distancia total: " << dist[destino] << " km\n";
    }

    void mostrarCiudades() {
        if (total == 0) { cout << "  [Sin ciudades registradas]\n"; return; }
        for (int i = 0; i < total; i++)
            cout << "  [" << i+1 << "] " << ciudades[i] << "\n";
    }

    void mostrarRutas() {
        cout << "\n  Red de rutas disponibles:\n";
        bool hayRutas = false;
        for (int i = 0; i < total; i++)
            for (int j = i+1; j < total; j++)
                if (matriz[i][j] != INF) {
                    cout << "  " << ciudades[i] << " ↔ " << ciudades[j]
                         << " (" << matriz[i][j] << " km)\n";
                    hayRutas = true;
                }
        if (!hayRutas) cout << "  [Sin rutas definidas]\n";
    }

    string getCiudad(int i) {
        if (i < 0 || i >= total) return "";
        return ciudades[i];
    }

    int getTotal() { return total; }
};
