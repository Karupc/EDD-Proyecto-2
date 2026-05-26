#include <iostream>
#include "estructuras.h"

int main() {
    ArbolCapas sistemaCapas;
    ListaImagenes sistemaImagenes;
    ArbolUsuarios sistemaUsuarios;

    int opcion = 0;

    while (opcion != 4) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "     GENERADOR DE IMÁGENES - MENU    " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Carga Masiva (Próximamente)" << std::endl;
        std::cout << "2. Gestión de Estructuras (Pruebas)" << std::endl;
        std::cout << "3. Generar Reportes Graphviz" << std::endl;
        std::cout << "4. Salir del programa" << std::endl;
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "\n[Info] Aquí procesaremos los archivos .cap, .im y .usr\n" << std::endl;
                break;
            case 2:
                std::cout << "\n[Info] Estructuras base inicializadas correctamente en memoria.\n" << std::endl;
                break;
            case 3:
                std::cout << "\n[Info] Aquí exportaremos el formato DOT para Graphviz.\n" << std::endl;
                break;
            case 4:
                std::cout << "\n¡Saliendo del sistema! Feliz día.\n" << std::endl;
                break;
            default:
                std::cout << "\n[Error] Opción no válida, intente de nuevo.\n" << std::endl;
        }
    }

    return 0;
}