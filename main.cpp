#include <iostream>
#include <string>
#include "estructuras.h"
#include "sistema.h"

int main() {
    ArbolCapas sistemaCapas;
    ListaImagenes sistemaImagenes;
    ArbolUsuarios sistemaUsuarios;
    CargaMasiva lector;

    int opcion = 0;

    while (opcion != 4) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "     GENERADOR DE IMÁGENES - MENU    " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Carga Masiva de datos" << std::endl;
        std::cout << "2. Gestión de Estructuras (Verificar Memoria)" << std::endl;
        std::cout << "3. Generar Reportes Graphviz" << std::endl;
        std::cout << "4. Salir del programa" << std::endl;
        std::cout << "Seleccione una opción: ";
        
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (opcion) {
            case 1: {
                std::string arcCapas, arcImagenes, arcUsuarios;
                
                std::cout << "\n--- CARGA MASIVA DE ARCHIVOS ---" << std::endl;
                
                std::cout << "Ingrese nombre del archivo de Capas (.cap): ";
                std::getline(std::cin, arcCapas);
                if (!arcCapas.empty()) {
                    lector.cargarCapas(arcCapas, sistemaCapas);
                }

                std::cout << "Ingrese nombre del archivo de Imagenes (.im): ";
                std::getline(std::cin, arcImagenes);
                if (!arcImagenes.empty()) {
                    lector.cargarImagenes(arcImagenes, sistemaImagenes, sistemaCapas);
                }

                std::cout << "Ingrese nombre del archivo de Usuarios (.usr): ";
                std::getline(std::cin, arcUsuarios);
                if (!arcUsuarios.empty()) {
                    lector.cargarUsuarios(arcUsuarios, sistemaUsuarios);
                }
                
                break;
            }
                
            case 2: {
                std::cout << "\n--- ESTADO ACTUAL DE LA MEMORIA ---" << std::endl;
                
                std::cout << "[Info] Verificando raiz de usuarios..." << std::endl;
                if (sistemaUsuarios.getRaiz() != nullptr) {
                    std::cout << "[OK] El arbol de usuarios ya tiene datos." << std::endl;
                } else {
                    std::cout << "[!] No hay usuarios cargados todavia." << std::endl;
                }

                if (sistemaImagenes.getPrimero() != nullptr) {
                    std::cout << "[OK] La lista de imagenes ya tiene datos." << std::endl;
                } else {
                    std::cout << "[!] No hay imagenes cargadas todavia." << std::endl;
                }
                break;
            }
                
            case 3:
                std::cout << "\n[Info] Esta opcion generara los reportes DOT mas adelante.\n" << std::endl;
                break;
            case 4:
                std::cout << "\n¡Saliendo del sistema! Feliz dia.\n" << std::endl;
                break;
            default:
                std::cout << "\n[Error] Opcion invalida.\n" << std::endl;
        }
    }

    return 0;
}