#pragma once
#include <string>
using namespace std;

// ─────────────────────────────────────────
//  ENTIDADES DEL SISTEMA DE ENVÍOS
// ─────────────────────────────────────────

struct Cliente {
    string id;
    string nombre;
    string telefono;
    string ciudad;
};

struct Paquete {
    string id;
    string descripcion;
    float  peso;
    string estado;
    string idCliente; 
};

struct Envio {
    string id;
    string idPaquete;
    string ciudadOrigen;
    string ciudadDestino;
    string fecha;
};
