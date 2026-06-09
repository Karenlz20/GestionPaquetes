#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "../Entidades/Structs.h"
#include "../Estructuras/Lista.h"
#include "../Estructuras/Queue.h"
#include "../Estructuras/Stack.h"
#include "../Estructuras/Matriz.h"
#include "../Estructuras/BST.h"
#include "../Estructuras/Grafo.h"
#include "../Persistencia/Archivo.h"
using namespace std;

class Sistema {
private:
    Lista<Cliente> clientes;
    Lista<Paquete> paquetes;
    Lista<Envio>   envios;

    Queue<Paquete> colaDespacho;
    Stack          historial;
    Matriz         almacen;
    BST            indicePaquetes;
    BST            indiceClientes;
    Grafo          grafo;

    int contClientes;
    int contPaquetes;
    int contEnvios;

    string generarId(string prefijo, int contador) {
        string num = to_string(contador);
        while ((int)num.length() < 3) num = "0" + num;
        return prefijo + num;
    }

    bool validarTelefono(string tel) {
        if (tel.length() < 7 || tel.length() > 10) return false;
        for (char c : tel)
            if (c < '0' || c > '9') return false;
        return true;
    }

    bool validarPeso(float peso) { return peso > 0 && peso <= 1000; }

    bool validarFecha(string fecha) {
        if (fecha.length() != 10) return false;
        if (fecha[2] != '/' || fecha[5] != '/') return false;
        return true;
    }

    void separador() { cout << "  " << string(50, '-') << "\n"; }

    string resolverId(string entrada, string prefijo) {
        bool esNumero = !entrada.empty();
        for (char c : entrada)
            if (c < '0' || c > '9') { esNumero = false; break; }
        if (esNumero)
            return indicePaquetes.numeroAId(prefijo, stoi(entrada));
        return entrada;
    }

    // Genera un número de fecha aleatorio válido
    string fechaAleatoria() {
        int dia  = 1  + rand() % 28;
        int mes  = 1  + rand() % 12;
        int anio = 2024 + rand() % 2;
        string d = to_string(dia);  if (d.length() < 2) d = "0" + d;
        string m = to_string(mes);  if (m.length() < 2) m = "0" + m;
        return d + "/" + m + "/" + to_string(anio);
    }

    // Mezcla un arreglo de enteros (Fisher-Yates)
    void mezclar(int* arr, int n) {
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    void listarClientesInorden(NodoBST* nodo) {
        if (!nodo) return;
        listarClientesInorden(nodo->izquierda);
        Cliente* c = clientes.buscar(nodo->id);
        if (c) {
            cout << "  " << left
                << setw(8)  << c->id
                << setw(22) << c->nombre
                << setw(12) << c->telefono
                << setw(15) << c->ciudad << "\n";
        }
        listarClientesInorden(nodo->derecha);
    }

    void listarPaquetesInorden(NodoBST* nodo) {
        if (!nodo) return;
        listarPaquetesInorden(nodo->izquierda);
        Paquete* p = paquetes.buscar(nodo->id);
        if (p) {
            cout << "  " << left
                << setw(8)  << p->id
                << setw(22) << p->descripcion
                << setw(8)  << p->peso
                << setw(15) << p->estado
                << setw(8)  << p->idCliente << "\n";
        }
        listarPaquetesInorden(nodo->derecha);
    }

public:
    Sistema() : contClientes(1), contPaquetes(1), contEnvios(1) {
        srand((unsigned int)time(nullptr));
        inicializarGrafo();
    }

    void inicializarGrafo() {
        grafo.agregarCiudad("Santa Cruz");
        grafo.agregarCiudad("La Paz");
        grafo.agregarCiudad("Cochabamba");
        grafo.agregarCiudad("Oruro");
        grafo.agregarCiudad("Sucre");
        grafo.agregarCiudad("Tarija");
        grafo.agregarCiudad("Potosi");
        grafo.agregarCiudad("Trinidad");

        grafo.agregarRuta("Santa Cruz",  "Cochabamba", 465);
        grafo.agregarRuta("Santa Cruz",  "Tarija",     320);
        grafo.agregarRuta("Santa Cruz",  "Trinidad",   530);
        grafo.agregarRuta("La Paz",      "Cochabamba", 394);
        grafo.agregarRuta("La Paz",      "Oruro",      230);
        grafo.agregarRuta("Cochabamba",  "Oruro",      210);
        grafo.agregarRuta("Cochabamba",  "Sucre",      340);
        grafo.agregarRuta("Oruro",       "Potosi",     160);
        grafo.agregarRuta("Potosi",      "Sucre",      160);
        grafo.agregarRuta("Sucre",       "Tarija",     390);
        grafo.agregarRuta("Potosi",      "Tarija",     480);
    }

    void cargarDatosPrueba() {
        // Datos fijos de nombres, teléfonos y ciudades
        string nombres[]   = { "Ana Lopez", "Carlos Rios", "Maria Flores",
                                "Luis Gutierrez", "Sofia Mendez", "Jorge Vargas",
                                "Carla Perez", "Diego Suarez" };
        string telefonos[] = { "76543210", "71234567", "69876543",
                                "78901234", "72345678", "65432109",
                                "79012345", "73456789" };
        string ciudades[]  = { "Santa Cruz", "La Paz", "Cochabamba",
                                "Oruro", "Sucre", "Tarija", "Potosi", "Trinidad" };

        string descripciones[] = { "Laptop Dell", "Ropa deportiva", "Libros",
                                    "Telefono Samsung", "Casco de moto", "Zapatos Nike",
                                    "Mochila Northface", "Camara Sony", "Auriculares JBL",
                                    "Tablet Lenovo" };
        float pesos[] = { 2.5f, 1.2f, 3.8f, 0.5f, 1.8f, 1.0f, 0.9f, 1.5f, 0.4f, 0.8f };

        // ── Clientes: 8 IDs mezclados aleatoriamente ──
        // Rango 1..8, mezclado para que la raiz del BST no sea 1
        int idsClientes[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        mezclar(idsClientes, 8);

        for (int i = 0; i < 8; i++) {
            int num = idsClientes[i];
            string idCompleto = indiceClientes.numeroAId("CLI", num);
            if (indiceClientes.buscar(idCompleto)) continue;

            Cliente c;
            c.id       = idCompleto;
            c.nombre   = nombres[i];
            c.telefono = telefonos[i];
            c.ciudad   = ciudades[i];

            clientes.agregar(c);
            indiceClientes.insertar(c.id);
            // contClientes se ajusta al máximo usado
            if (num >= contClientes) contClientes = num + 1;
            historial.push("INSERT", "Clientes", c.id, "Prueba: " + c.nombre);
        }

        // ── Paquetes: 10 IDs mezclados aleatoriamente ──
        int idsPaquetes[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        mezclar(idsPaquetes, 10);

        // Recolectar IDs de clientes existentes para asignarlos
        string idsClientesRegistrados[8];
        NodoLista<Cliente>* cAux = clientes.getCabeza();
        int ci = 0;
        while (cAux && ci < 8) {
            idsClientesRegistrados[ci++] = cAux->dato.id;
            cAux = cAux->siguiente;
        }

        for (int i = 0; i < 10; i++) {
            int num = idsPaquetes[i];
            string idCompleto = indicePaquetes.numeroAId("PAQ", num);
            if (indicePaquetes.buscar(idCompleto)) continue;

            Paquete p;
            p.id          = idCompleto;
            p.descripcion = descripciones[i];
            p.peso        = pesos[i];
            p.estado      = "Pendiente";
            p.idCliente   = idsClientesRegistrados[i % ci];

            paquetes.agregar(p);
            indicePaquetes.insertar(p.id);
            almacen.asignar(p.id);
            colaDespacho.encolar(p);
            if (num >= contPaquetes) contPaquetes = num + 1;
            historial.push("INSERT", "Paquetes", p.id, "Prueba: " + p.descripcion);
        }

        // ── Envios: 4 aleatorios entre los paquetes registrados ──
        string ciudadesEnvio[] = { "Santa Cruz", "La Paz", "Cochabamba",
                                    "Oruro", "Sucre", "Tarija" };

        string idsPaqRegistrados[10];
        NodoLista<Paquete>* pAux = paquetes.getCabeza();
        int pi = 0;
        while (pAux && pi < 10) {
            idsPaqRegistrados[pi++] = pAux->dato.id;
            pAux = pAux->siguiente;
        }

        // Mezclar para elegir 4 paquetes distintos
        int indEnvios[10] = {0,1,2,3,4,5,6,7,8,9};
        mezclar(indEnvios, pi);

        for (int i = 0; i < 4 && i < pi; i++) {
            string idPaq = idsPaqRegistrados[indEnvios[i]];
            int orig = rand() % 6;
            int dest = rand() % 6;
            if (dest == orig) dest = (dest + 1) % 6;

            Envio env;
            env.id            = generarId("ENV", contEnvios++);
            env.idPaquete     = idPaq;
            env.ciudadOrigen  = ciudadesEnvio[orig];
            env.ciudadDestino = ciudadesEnvio[dest];
            env.fecha         = fechaAleatoria();

            envios.agregar(env);
            Paquete* p = paquetes.buscar(idPaq);
            if (p) p->estado = "En transito";
            historial.push("INSERT", "Envios", env.id,
                           idPaq + ": " + env.ciudadOrigen + " -> " + env.ciudadDestino);
        }
    }

    // ════════════════════════════════════
    //  CRUD CLIENTES
    // ════════════════════════════════════

    bool registrarCliente(string nombre, string telefono, string ciudad, string idManual = "") {
        if (nombre.empty())             { cout << "  [!] El nombre no puede estar vacio.\n";               return false; }
        if (!validarTelefono(telefono)) { cout << "  [!] Telefono invalido (7-10 digitos numericos).\n";   return false; }
        if (ciudad.empty())             { cout << "  [!] La ciudad no puede estar vacia.\n";               return false; }

        Cliente c;
        if (idManual.empty()) {
            c.id = generarId("CLI", contClientes++);
        } else {
            string idCompleto = indiceClientes.numeroAId("CLI", stoi(idManual));
            if (indiceClientes.buscar(idCompleto)) {
                cout << "  [!] Ya existe un cliente con ese numero.\n";
                return false;
            }
            c.id = idCompleto;
            contClientes++;
        }
        c.nombre   = nombre;
        c.telefono = telefono;
        c.ciudad   = ciudad;

        clientes.agregar(c);
        indiceClientes.insertar(c.id);
        historial.push("INSERT", "Clientes", c.id, "Nuevo cliente: " + nombre);
        cout << "  OK Cliente registrado con ID: " << c.id << "\n";
        return true;
    }

    void listarClientes() {
        if (clientes.estaVacia()) { cout << "  [Sin clientes]\n"; return; }
        cout << "\n  " << left
            << setw(8)  << "ID"
            << setw(22) << "Nombre"
            << setw(12) << "Telefono"
            << setw(15) << "Ciudad" << "\n";
        separador();
        listarClientesInorden(indiceClientes.getRaiz());
    }

    void buscarCliente(string entrada) {
        string id = resolverId(entrada, "CLI");
        cout << "  Buscando en BST: " << entrada << " -> ID: " << id << "\n";
        if (!indiceClientes.buscar(id)) {
            cout << "  [!] ID '" << id << "' no encontrado en el indice BST.\n";
            return;
        }
        Cliente* c = clientes.buscar(id);
        if (!c) return;

        cout << "\n  -- Cliente --\n";
        cout << "  ID:       " << c->id       << "\n";
        cout << "  Nombre:   " << c->nombre   << "\n";
        cout << "  Telefono: " << c->telefono << "\n";
        cout << "  Ciudad:   " << c->ciudad   << "\n";

        cout << "\n  -- Paquetes asociados (1:N) --\n";
        NodoLista<Paquete>* p = paquetes.getCabeza();
        bool tiene = false;
        while (p) {
            if (p->dato.idCliente == id) {
                cout << "  |-- " << p->dato.id << " - " << p->dato.descripcion
                     << " [" << p->dato.estado << "]\n";
                tiene = true;
            }
            p = p->siguiente;
        }
        if (!tiene) cout << "  |-- (sin paquetes)\n";
    }

    bool actualizarCliente(string entrada, string nuevoTel, string nuevaCiudad) {
        string id = resolverId(entrada, "CLI");
        Cliente* c = clientes.buscar(id);
        if (!c) { cout << "  [!] Cliente no encontrado.\n"; return false; }
        if (!validarTelefono(nuevoTel)) { cout << "  [!] Telefono invalido.\n"; return false; }
        string detalle = "tel " + c->telefono + " -> " + nuevoTel;
        c->telefono = nuevoTel;
        c->ciudad   = nuevaCiudad;
        historial.push("UPDATE", "Clientes", id, detalle);
        cout << "  OK Cliente actualizado.\n";
        return true;
    }

    bool eliminarCliente(string entrada) {
        string id = resolverId(entrada, "CLI");
        NodoLista<Paquete>* actual = paquetes.getCabeza();
        while (actual) {
            if (actual->dato.idCliente == id) {
                cout << "  [!] No se puede eliminar: el cliente tiene paquetes asociados.\n";
                return false;
            }
            actual = actual->siguiente;
        }
        if (!clientes.eliminar(id)) { cout << "  [!] Cliente no encontrado.\n"; return false; }
        indiceClientes.eliminar(id);
        historial.push("DELETE", "Clientes", id, "Cliente eliminado");
        cout << "  OK Cliente eliminado.\n";
        return true;
    }

    // ════════════════════════════════════
    //  CRUD PAQUETES
    // ════════════════════════════════════

    bool registrarPaquete(string descripcion, float peso, string idClienteEntrada, string idManual = "") {
        if (descripcion.empty()) { cout << "  [!] La descripcion no puede estar vacia.\n"; return false; }
        if (!validarPeso(peso))  { cout << "  [!] Peso invalido (0-1000 kg).\n"; return false; }

        string idCliente = resolverId(idClienteEntrada, "CLI");
        if (!clientes.existe(idCliente)) { cout << "  [!] El cliente '" << idCliente << "' no existe.\n"; return false; }

        Paquete p;
        if (idManual.empty()) {
            p.id = generarId("PAQ", contPaquetes++);
        } else {
            string idCompleto = indicePaquetes.numeroAId("PAQ", stoi(idManual));
            if (indicePaquetes.buscar(idCompleto)) {
                cout << "  [!] Ya existe un paquete con ese numero.\n";
                return false;
            }
            p.id = idCompleto;
            contPaquetes++;
        }
        p.descripcion = descripcion;
        p.peso        = peso;
        p.estado      = "Pendiente";
        p.idCliente   = idCliente;

        paquetes.agregar(p);
        indicePaquetes.insertar(p.id);
        almacen.asignar(p.id);
        colaDespacho.encolar(p);
        historial.push("INSERT", "Paquetes", p.id, "Nuevo paquete: " + descripcion);
        cout << "  OK Paquete registrado con ID: " << p.id << "\n";
        return true;
    }

    void listarPaquetes() {
        if (paquetes.estaVacia()) { cout << "  [Sin paquetes]\n"; return; }
        cout << "\n  " << left
            << setw(8)  << "ID"
            << setw(22) << "Descripcion"
            << setw(8)  << "Peso"
            << setw(15) << "Estado"
            << setw(8)  << "Cliente" << "\n";
        separador();
        listarPaquetesInorden(indicePaquetes.getRaiz());
    }

    void buscarPaquete(string entrada) {
        string idBuscado = resolverId(entrada, "PAQ");
        cout << "  Buscando en BST: " << entrada << " -> ID: " << idBuscado << "\n";
        if (!indicePaquetes.buscar(idBuscado)) {
            cout << "  [!] ID '" << idBuscado << "' no encontrado en el indice BST.\n";
            return;
        }
        Paquete* p = paquetes.buscar(idBuscado);
        if (!p) return;

        cout << "\n  -- Paquete --\n";
        cout << "  ID:          " << p->id          << "\n";
        cout << "  Descripcion: " << p->descripcion << "\n";
        cout << "  Peso:        " << p->peso        << " kg\n";
        cout << "  Estado:      " << p->estado      << "\n";
        cout << "  Cliente:     " << p->idCliente   << "\n";

        Cliente* c = clientes.buscar(p->idCliente);
        if (c) {
            cout << "\n  -- Cliente relacionado (N:1) --\n";
            cout << "  Nombre:   " << c->nombre   << "\n";
            cout << "  Telefono: " << c->telefono << "\n";
            cout << "  Ciudad:   " << c->ciudad   << "\n";
        }

        NodoLista<Envio>* e = envios.getCabeza();
        while (e) {
            if (e->dato.idPaquete == idBuscado) {
                cout << "\n  -- Envio relacionado (1:1) --\n";
                cout << "  ID Envio:  " << e->dato.id            << "\n";
                cout << "  Origen:    " << e->dato.ciudadOrigen  << "\n";
                cout << "  Destino:   " << e->dato.ciudadDestino << "\n";
                cout << "  Fecha:     " << e->dato.fecha         << "\n";
                break;
            }
            e = e->siguiente;
        }
    }

    bool actualizarEstadoPaquete(string entrada, string nuevoEstado) {
        string id = resolverId(entrada, "PAQ");
        if (nuevoEstado != "Pendiente" && nuevoEstado != "En transito" && nuevoEstado != "Entregado") {
            cout << "  [!] Estado invalido. Opciones: Pendiente | En transito | Entregado\n";
            return false;
        }
        Paquete* p = paquetes.buscar(id);
        if (!p) { cout << "  [!] Paquete no encontrado.\n"; return false; }
        string detalle = p->estado + " -> " + nuevoEstado;
        p->estado = nuevoEstado;
        historial.push("UPDATE", "Paquetes", id, detalle);
        cout << "  OK Estado actualizado: " << detalle << "\n";
        return true;
    }

    bool eliminarPaquete(string entrada) {
        string id = resolverId(entrada, "PAQ");
        NodoLista<Envio>* e = envios.getCabeza();
        while (e) {
            if (e->dato.idPaquete == id) {
                cout << "  [!] No se puede eliminar: el paquete tiene un envio asociado.\n";
                return false;
            }
            e = e->siguiente;
        }
        if (!paquetes.eliminar(id)) { cout << "  [!] Paquete no encontrado.\n"; return false; }
        indicePaquetes.eliminar(id);
        almacen.liberar(id);
        historial.push("DELETE", "Paquetes", id, "Paquete eliminado");
        cout << "  OK Paquete eliminado.\n";
        return true;
    }

    // ════════════════════════════════════
    //  CRUD ENVIOS
    // ════════════════════════════════════

    bool registrarEnvio(string idPaqueteEntrada, string origen, string destino, string fecha) {
        string idPaquete = resolverId(idPaqueteEntrada, "PAQ");
        if (!paquetes.existe(idPaquete)) { cout << "  [!] El paquete '" << idPaquete << "' no existe.\n"; return false; }
        NodoLista<Envio>* e = envios.getCabeza();
        while (e) {
            if (e->dato.idPaquete == idPaquete) {
                cout << "  [!] El paquete ya tiene un envio registrado (relacion 1:1).\n";
                return false;
            }
            e = e->siguiente;
        }
        if (!validarFecha(fecha)) { cout << "  [!] Fecha invalida. Formato: DD/MM/AAAA\n"; return false; }

        Envio env;
        env.id            = generarId("ENV", contEnvios++);
        env.idPaquete     = idPaquete;
        env.ciudadOrigen  = origen;
        env.ciudadDestino = destino;
        env.fecha         = fecha;

        envios.agregar(env);
        Paquete* p = paquetes.buscar(idPaquete);
        if (p) p->estado = "En transito";
        historial.push("INSERT", "Envios", env.id, idPaquete + ": " + origen + " -> " + destino);
        cout << "  OK Envio registrado con ID: " << env.id << "\n";
        return true;
    }

    void listarEnvios() {
        if (envios.estaVacia()) { cout << "  [Sin envios]\n"; return; }
        cout << "\n  " << left
             << setw(8)  << "ID"
             << setw(8)  << "Paquete"
             << setw(15) << "Origen"
             << setw(15) << "Destino"
             << setw(12) << "Fecha" << "\n";
        separador();
        NodoLista<Envio>* actual = envios.getCabeza();
        while (actual) {
            Envio& env = actual->dato;
            cout << "  " << left
                 << setw(8)  << env.id
                 << setw(8)  << env.idPaquete
                 << setw(15) << env.ciudadOrigen
                 << setw(15) << env.ciudadDestino
                 << setw(12) << env.fecha << "\n";
            actual = actual->siguiente;
        }
    }

    // ════════════════════════════════════
    //  VISUALIZACION DE ESTRUCTURAS
    // ════════════════════════════════════

    void verLista() {
        cout << "\n  == LISTA ENLAZADA ==\n";
        cout << "  Principio: cada nodo apunta al siguiente\n";
        cout << "  Uso en este sistema: catalogo de paquetes\n\n";
        if (paquetes.estaVacia()) { cout << "  [Lista vacia]\n"; return; }
        NodoLista<Paquete>* actual = paquetes.getCabeza();
        int i = 1;
        while (actual) {
            cout << "  [Nodo " << i++ << "] " << actual->dato.id
                 << " - " << actual->dato.descripcion << " -> ";
            if (actual->siguiente) cout << actual->siguiente->dato.id << "\n";
            else                   cout << "nullptr (fin)\n";
            actual = actual->siguiente;
        }
    }

    void verColaDespacho() {
        cout << "\n  == QUEUE (Cola FIFO) ==\n";
        cout << "  Principio: primero en entrar, primero en salir\n";
        cout << "  Uso en este sistema: paquetes esperando despacho\n\n";
        if (colaDespacho.estaVacia()) { cout << "  [Cola vacia]\n"; return; }
        cout << "  FRENTE (sale primero)\n      |\n";
        NodoQueue<Paquete>* actual = colaDespacho.getFrente();
        int i = 1;
        while (actual) {
            cout << "  [" << i++ << "] " << actual->dato.id
                 << " - " << actual->dato.descripcion;
            if (!actual->siguiente) cout << "  <- FIN (entra aqui)";
            cout << "\n";
            actual = actual->siguiente;
        }
    }

    void despacharSiguiente() {
        if (colaDespacho.estaVacia()) { cout << "  [!] Cola vacia.\n"; return; }
        Paquete p;
        colaDespacho.desencolar(p);
        almacen.liberar(p.id);
        Paquete* enLista = paquetes.buscar(p.id);
        if (enLista) enLista->estado = "En transito";
        historial.push("DESPACHO", "Paquetes", p.id, p.descripcion + " despachado");
        cout << "  OK Despachado: " << p.id << " - " << p.descripcion << "\n";
    }

    void verHistorial() {
        cout << "\n  == STACK (Pila LIFO) ==\n";
        cout << "  Principio: ultima en entrar, primera en salir\n";
        cout << "  Uso en este sistema: historial UNDO de operaciones\n\n";
        historial.mostrar();
    }

    void deshacerUltima() {
        Operacion op;
        if (!historial.pop(op)) { cout << "  [!] No hay operaciones para deshacer.\n"; return; }
        cout << "  OK Deshecha: [" << op.tipo << "] "
             << op.entidad << " | " << op.detalle << "\n";
    }

    void verAlmacen() {
        cout << "\n  == MATRIZ ==\n";
        cout << "  Principio: estructura de filas x columnas\n";
        cout << "  Uso en este sistema: mapa fisico del almacen\n";
        almacen.mostrar();
        cout << "  Ocupados: " << almacen.getOcupados()
             << " / " << almacen.getCapacidad() << "\n";
    }

    void verBSTPaquetes() {
        int op;
        do {
            cout << "\n  == BST DE PAQUETES ==\n";
            cout << "  Principio: nodos menores izquierda, mayores derecha\n";
            cout << "  ┌── hijo derecho (mayor)\n";
            cout << "  └── hijo izquierdo (menor)\n\n";
            cout << " [1] Estructura visual del arbol\n";
            cout << " [2] Inorden   (izq -> raiz -> der)  — resultado ordenado\n";
            cout << " [3] Preorden  (raiz -> izq -> der)  — raiz primero\n";
            cout << " [4] Postorden (izq -> der -> raiz)  — raiz al final\n";
            cout << " [5] Ver todo\n";
            cout << " [0] Volver\n";
            cout << "-----------------------------------------------------\n";
            cout << " Opcion: ";
            cin >> op; cin.ignore();
            cout << "\n";

            if      (op == 1) indicePaquetes.mostrarArbol();
            else if (op == 2) indicePaquetes.mostrarInorden();
            else if (op == 3) indicePaquetes.mostrarPreorden();
            else if (op == 4) indicePaquetes.mostrarPostorden();
            else if (op == 5) indicePaquetes.mostrarTodo();

            if (op != 0) {
                cout << "\n  Presione Enter para continuar...";
                cin.get();
            }
        } while (op != 0);
    }

    void verBSTClientes() {
        int op;
        do {
            cout << "\n  == BST DE CLIENTES ==\n";
            cout << "  Principio: nodos menores izquierda, mayores derecha\n";
            cout << "  ┌── hijo derecho (mayor)\n";
            cout << "  └── hijo izquierdo (menor)\n\n";
            cout << " [1] Estructura visual del arbol\n";
            cout << " [2] Inorden   (izq -> raiz -> der)  — resultado ordenado\n";
            cout << " [3] Preorden  (raiz -> izq -> der)  — raiz primero\n";
            cout << " [4] Postorden (izq -> der -> raiz)  — raiz al final\n";
            cout << " [5] Ver todo\n";
            cout << " [0] Volver\n";
            cout << "-----------------------------------------------------\n";
            cout << " Opcion: ";
            cin >> op; cin.ignore();
            cout << "\n";

            if      (op == 1) indiceClientes.mostrarArbol();
            else if (op == 2) indiceClientes.mostrarInorden();
            else if (op == 3) indiceClientes.mostrarPreorden();
            else if (op == 4) indiceClientes.mostrarPostorden();
            else if (op == 5) indiceClientes.mostrarTodo();

            if (op != 0) {
                cout << "\n  Presione Enter para continuar...";
                cin.get();
            }
        } while (op != 0);
    }

    void verRutas() {
        cout << "\n  == GRAFO ==\n";
        cout << "  Principio: nodos (ciudades) conectados por aristas (rutas con km)\n";
        cout << "  Uso en este sistema: red de rutas entre ciudades\n\n";
        grafo.mostrarRutas();
    }

    void calcularRuta() {
        cout << "\n  == GRAFO + DIJKSTRA ==\n";
        cout << "  Calcula la ruta mas corta entre dos ciudades\n\n";
        grafo.mostrarCiudades();
        int orig, dest;
        cout << "\n  Ciudad origen  [numero]: "; cin >> orig;
        cout << "  Ciudad destino [numero]: "; cin >> dest;
        cin.ignore();
        grafo.dijkstra(orig - 1, dest - 1);
    }

    void verRelaciones() {
        cout << "\n  == RELACIONES ENTRE ENTIDADES ==\n\n";
        cout << "  Cliente  1:N  Paquetes\n";
        cout << "  Paquete  1:1  Envio\n";
        cout << "  Ciudad   1:N  Envios\n";

        cout << "\n=====================================================\n";
        cout << "  -- CLIENTES Y SUS PAQUETES (1:N) --\n";
        cout << "=====================================================\n";
        NodoLista<Cliente>* c = clientes.getCabeza();
        while (c) {
            cout << "\n  Cliente: " << c->dato.id << " - " << c->dato.nombre << "\n";
            NodoLista<Paquete>* p = paquetes.getCabeza();
            bool tiene = false;
            while (p) {
                if (p->dato.idCliente == c->dato.id) {
                    cout << "    |-- " << p->dato.id << " - "
                         << p->dato.descripcion << " [" << p->dato.estado << "]\n";
                    tiene = true;
                }
                p = p->siguiente;
            }
            if (!tiene) cout << "    |-- (sin paquetes)\n";
            c = c->siguiente;
            cout << "--------------------------------------------------\n";
        }

        cout << "\n=====================================================\n";
        cout << "  -- PAQUETES Y SUS ENVIOS (1:1) --\n";
        cout << "=====================================================\n";
        NodoLista<Paquete>* p = paquetes.getCabeza();
        while (p) {
            cout << "\n  Paquete: " << p->dato.id << " - " << p->dato.descripcion << "\n";
            NodoLista<Envio>* e = envios.getCabeza();
            bool tiene = false;
            while (e) {
                if (e->dato.idPaquete == p->dato.id) {
                    cout << "    |-- " << e->dato.id << " | "
                         << e->dato.ciudadOrigen << " -> "
                         << e->dato.ciudadDestino << " | "
                         << e->dato.fecha << "\n";
                    tiene = true;
                }
                e = e->siguiente;
            }
            if (!tiene) cout << "    |-- (sin envio)\n";
            p = p->siguiente;
            cout << "--------------------------------------------------\n";
        }
    }

    // ════════════════════════════════════
    //  PERSISTENCIA
    // ════════════════════════════════════

    void guardar() {
        bool ok = true;
        ok &= Archivo::guardarClientes(clientes);
        ok &= Archivo::guardarPaquetes(paquetes);
        ok &= Archivo::guardarEnvios(envios);
        if (ok) cout << "  OK Datos guardados en clientes.txt, paquetes.txt, envios.txt\n";
    }

    void cargar() {
        bool ok = true;
        ok &= Archivo::cargarClientes(clientes);
        ok &= Archivo::cargarPaquetes(paquetes);
        ok &= Archivo::cargarEnvios(envios);
        if (ok) cout << "  OK Datos cargados desde archivos .txt\n";
        else    cout << "  [!] No se encontraron archivos previos.\n";
    }
};
