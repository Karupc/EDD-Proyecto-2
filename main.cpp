#include <iostream>
#include <string>
#include "estructuras.h"
#include "sistema.h" 

int main() {
    system("chcp 65001 > nul");

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
                
                // 1. Verificar Árbol de Usuarios
                std::cout << "\n[1/3] Verificando Árbol de Usuarios..." << std::endl;
                NodoUsuario* raizUsr = sistemaUsuarios.getRaiz();
                if (raizUsr != nullptr) {
                    std::cout << "  -> [OK] El árbol de usuarios tiene una raíz válida." << std::endl;
                    std::cout << "  -> Usuario raíz detectado: " << raizUsr->nombre << std::endl;
                } else {
                    std::cout << "  -> [!] El árbol de usuarios está VACÍO." << std::endl;
                }

                // 2. Verificar Lista de Imágenes
                std::cout << "\n[2/3] Verificando Lista de Imágenes..." << std::endl;
                NodoImagen* primeroImg = sistemaImagenes.getPrimero();
                if (primeroImg != nullptr) {
                    std::cout << "  -> [OK] La lista de imágenes tiene datos en memoria." << std::endl;
                    std::cout << "  -> Primera imagen en la lista: ID " << primeroImg->idImagen << std::endl;
                } else {
                    std::cout << "  -> [!] La lista de imágenes está VACÍA." << std::endl;
                }

                // 3. Verificar Árbol de Capas y sus Matrices
                std::cout << "\n[3/3] Verificando Árbol de Capas..." << std::endl;
                NodoArbolCapas* raizCapas = sistemaCapas.getRaiz();
                if (raizCapas != nullptr) {
                    std::cout << "  -> [OK] El árbol de capas tiene datos en memoria." << std::endl;
                    std::cout << "  -> Capa en la raíz del árbol: ID " << raizCapas->idCapa << std::endl;
                    if (raizCapas->raizMatriz != nullptr) {
                        std::cout << "     [Matriz] Esta capa ya inicializó su Matriz Dispersa." << std::endl;
                    } else {
                        std::cout << "     [Matriz] Esta capa aún no tiene píxeles en su matriz." << std::endl;
                    }
                } else {
                    std::cout << "  -> [!] El árbol de capas está VACÍO." << std::endl;
                }
                
                std::cout << "\n====================================" << std::endl;
                break;
            }
                
            case 3: {
                std::cout << "\n========================================" << std::endl;
                std::cout << "     MENU DE REPORTES - ACTUALIZADO     " << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "1. Reporte del Arbol de Usuarios" << std::endl;
                std::cout << "2. Reporte del Arbol de Capas" << std::endl;
                std::cout << "3. Reporte de la Lista de Imagenes" << std::endl; // <-- NUEVA LÍNEA
                std::cout << "Seleccione que reporte desea generar: ";
                
                int subOpcion = 0;
                std::cin >> subOpcion;
                std::cin.ignore(10000, '\n');

                if (subOpcion == 1) {
                    sistemaUsuarios.generarReporte();
                } else if (subOpcion == 2) {
                    sistemaCapas.generarReporte();
                } else if (subOpcion == 3) {
                    sistemaImagenes.generarReporte(); // <-- NUEVA ACCIÓN
                } else {
                    std::cout << "[!] Opcion invalida, regresando..." << std::endl;
                }
                break;
            }

            case 4:
                std::cout << "\n¡Saliendo del sistema! Feliz dia.\n" << std::endl;
                break;
            default:
                std::cout << "\n[Error] Opcion invalida.\n" << std::endl;
        }
    }

    return 0;
}