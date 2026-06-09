#include <iostream>
#include <string>
#include "Sistema/Sistema.h"
using namespace std;

Sistema sistema;

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\n  Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}

string leerCadena(string prompt) {
    string val;
    cout << "  " << prompt;
    getline(cin, val);
    return val;
}

float leerFloat(string prompt) {
    float val;
    cout << "  " << prompt;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  [!] Ingrese un numero valido: ";
    }
    cin.ignore();
    return val;
}

// ── SUBMENÚ CLIENTES ─────────────────────

void menuClientes() {
    int op;
    do {
        limpiarPantalla();
        cout << "=====================================================\n";
        cout << "                 GESTION CLIENTES\n";
        cout << "=====================================================\n";
        cout << " [1] Registrar cliente\n";
        cout << " [2] Listar clientes\n";
        cout << " [3] Buscar cliente por numero (BST)\n";
        cout << " [4] Actualizar cliente\n";
        cout << " [5] Eliminar cliente\n";
        cout << " [0] Volver\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();
        limpiarPantalla();

        if (op == 1) {
            string nom = leerCadena("Nombre: ");
            string tel = leerCadena("Telefono: ");
            string ciu = leerCadena("Ciudad: ");
            cout << "  Numero de cliente (Enter para automatico): ";
            string idMan; getline(cin, idMan);
            sistema.registrarCliente(nom, tel, ciu, idMan);
        } else if (op == 2) {
            sistema.listarClientes();
        } else if (op == 3) {
            cout << "  Ingrese el numero del cliente (ej: 2 para CLI002): ";
            string id; getline(cin, id);
            sistema.buscarCliente(id);
        } else if (op == 4) {
            sistema.listarClientes();
            string id  = leerCadena("Numero o ID del cliente: ");
            string tel = leerCadena("Nuevo telefono: ");
            string ciu = leerCadena("Nueva ciudad: ");
            sistema.actualizarCliente(id, tel, ciu);
        } else if (op == 5) {
            sistema.listarClientes();
            string id = leerCadena("Numero o ID del cliente: ");
            sistema.eliminarCliente(id);
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ── SUBMENÚ PAQUETES ─────────────────────

void menuPaquetes() {
    int op;
    do {
        limpiarPantalla();
        cout << "=====================================================\n";
        cout << "                 GESTION PAQUETES\n";
        cout << "=====================================================\n";
        cout << " [1] Registrar paquete\n";
        cout << " [2] Listar paquetes\n";
        cout << " [3] Buscar paquete por numero (BST)\n";
        cout << " [4] Actualizar estado\n";
        cout << " [5] Eliminar paquete\n";
        cout << " [0] Volver\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();
        limpiarPantalla();

        if (op == 1) {
            sistema.listarClientes();
            string desc  = leerCadena("Descripcion: ");
            float  peso  = leerFloat("Peso (kg): ");
            string idCli = leerCadena("Numero o ID del cliente: ");
            cout << "  Numero de paquete (Enter para automatico): ";
            string idMan; getline(cin, idMan);
            sistema.registrarPaquete(desc, peso, idCli, idMan);
        } else if (op == 2) {
            sistema.listarPaquetes();
        } else if (op == 3) {
            cout << "  Ingrese el numero del paquete (ej: 5 para PAQ005): ";
            string id; getline(cin, id);
            sistema.buscarPaquete(id);
        } else if (op == 4) {
            sistema.listarPaquetes();
            string id  = leerCadena("Numero o ID del paquete: ");
            cout << "  Estados: Pendiente | En transito | Entregado\n";
            string est = leerCadena("Nuevo estado: ");
            sistema.actualizarEstadoPaquete(id, est);
        } else if (op == 5) {
            sistema.listarPaquetes();
            string id = leerCadena("Numero o ID del paquete: ");
            sistema.eliminarPaquete(id);
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ── SUBMENÚ ENVÍOS ───────────────────────

void menuEnvios() {
    int op;
    do {
        limpiarPantalla();
        cout << "=====================================================\n";
        cout << "                  GESTION ENVIOS\n";
        cout << "=====================================================\n";
        cout << " [1] Registrar envio\n";
        cout << " [2] Listar envios\n";
        cout << " [3] Ver cola de despacho (Queue)\n";
        cout << " [4] Despachar siguiente\n";
        cout << " [0] Volver\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();
        limpiarPantalla();

        if (op == 1) {
            sistema.listarPaquetes();
            string idPaq = leerCadena("Numero o ID del paquete: ");
            sistema.verRutas();
            string orig  = leerCadena("Ciudad origen: ");
            string dest  = leerCadena("Ciudad destino: ");
            string fecha = leerCadena("Fecha (DD/MM/AAAA): ");
            sistema.registrarEnvio(idPaq, orig, dest, fecha);
        } else if (op == 2) {
            sistema.listarEnvios();
        } else if (op == 3) {
            sistema.verColaDespacho();
        } else if (op == 4) {
            sistema.despacharSiguiente();
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ── SUBMENÚ ESTRUCTURAS ──────────────────

void menuEstructuras() {
    int op;
    do {
        limpiarPantalla();
        cout << "=====================================================\n";
        cout << "               ESTRUCTURAS DE DATOS\n";
        cout << "=====================================================\n";
        cout << " [1] Lista enlazada\n";
        cout << " [2] Queue - cola de despacho\n";
        cout << " [3] Stack - historial UNDO\n";
        cout << " [4] Deshacer ultima operacion\n";
        cout << " [5] Matriz - mapa del almacen\n";
        cout << " [6] BST - arbol de paquetes\n";
        cout << " [7] BST - arbol de clientes\n";
        cout << " [8] Grafo - red de rutas\n";
        cout << " [9] Calcular ruta mas corta\n";
        cout << " [10] Ver relaciones entre tablas\n";
        cout << " [0] Volver\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();
        limpiarPantalla();

        if      (op == 1)  sistema.verLista();
        else if (op == 2)  sistema.verColaDespacho();
        else if (op == 3)  sistema.verHistorial();
        else if (op == 4)  sistema.deshacerUltima();
        else if (op == 5)  sistema.verAlmacen();
        else if (op == 6)  sistema.verBSTPaquetes();
        else if (op == 7)  sistema.verBSTClientes();
        else if (op == 8)  sistema.verRutas();
        else if (op == 9)  sistema.calcularRuta();
        else if (op == 10) sistema.verRelaciones();
        if (op != 0 && op != 6 && op != 7) pausar();
    } while (op != 0);
}

// ── SUBMENÚ PERSISTENCIA ─────────────────

void menuPersistencia() {
    int op;
    do {
        limpiarPantalla();
        cout << "=====================================================\n";
        cout << "                    PERSISTENCIA\n";
        cout << "=====================================================\n";
        cout << " [1] Guardar datos en archivos .txt\n";
        cout << " [2] Cargar datos desde archivos .txt\n";
        cout << " [3] Cargar datos de prueba\n";
        cout << " [0] Volver\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();
        limpiarPantalla();

        if      (op == 1) sistema.guardar();
        else if (op == 2) sistema.cargar();
        else if (op == 3) {
            sistema.cargarDatosPrueba();
            cout << "  OK Datos de prueba cargados.\n";
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ── MENÚ PRINCIPAL ───────────────────────

int main() {
    system("chcp 65001 > nul");
    int op;
    do {
        limpiarPantalla();
        cout << "\n";
        cout << "=====================================================\n";
        cout << "         SISTEMA DE GESTION DE ENVIOS\n";
        cout << "=====================================================\n";
        cout << " [1] Clientes\n";
        cout << " [2] Paquetes\n";
        cout << " [3] Envios\n";
        cout << " [4] Estructuras de datos\n";
        cout << " [5] Persistencia\n";
        cout << " [0] Salir\n";
        cout << "-----------------------------------------------------\n";
        cout << " Opcion: ";
        cin >> op; cin.ignore();

        if      (op == 1) menuClientes();
        else if (op == 2) menuPaquetes();
        else if (op == 3) menuEnvios();
        else if (op == 4) menuEstructuras();
        else if (op == 5) menuPersistencia();
        else if (op == 0) { limpiarPantalla(); cout << "\n  Hasta luego!\n\n"; }
    } while (op != 0);
    return 0;
}
