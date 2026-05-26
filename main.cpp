#include <iostream>
#include "estructuras.h"

int main() {
    
    ArbolCapas sistemaCapas;
    ListaImagenes sistemaImagenes;
    ArbolUsuarios sistemaUsuarios;

    int opcion = 0;

    while (opcion != 4) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "     GENERADOR DE IMAGENES - MENU    " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Carga Masiva (Proximamente)" << std::endl;
        std::cout << "2. Gestion de Estructuras (PROBAR MEMORIA)" << std::endl;
        std::cout << "3. Generar Reportes Graphviz" << std::endl;
        std::cout << "4. Salir del programa" << std::endl;
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "\n[Info] Aquí procesaremos los archivos .cap, .im y .usr\n" << std::endl;
                break;
                
            case 2: {
                std::cout << "\n--- CORRIENDO PRUEBA DE ESTRUCTURAS ---" << std::endl;
                
                // 1. Probar Árbol de Usuarios
                std::cout << "[->] Insertando usuario: 'Maria'..." << std::endl;
                sistemaUsuarios.insertar("Maria");
                NodoUsuario* u = sistemaUsuarios.buscar("Maria");
                if (u != nullptr) {
                    std::cout << "[OK] Usuario encontrado en el árbol: " << u->nombre << std::endl;
                }

                // 2. Probar Lista de Imágenes
                std::cout << "[->] Insertando Imagen ID: 10..." << std::endl;
                sistemaImagenes.insertar(10);
                NodoImagen* img = sistemaImagenes.buscar(10);
                if (img != nullptr) {
                    std::cout << "[OK] Imagen 10 creada en la Lista Circular." << std::endl;
                }

                // 3. Probar Árbol de Capas e Inserción de Píxeles en la Matriz Dispersa
                std::cout << "[->] Creando Capa ID: 3..." << std::endl;
                sistemaCapas.insertar(3);
                NodoArbolCapas* capa = sistemaCapas.buscar(3);
                
                if (capa != nullptr && capa->raizMatriz != nullptr) {
                    std::cout << "[OK] Capa 3 creada con su Matriz Dispersa inicializada." << std::endl;
                    
                    
                    std::cout << "[->] Insertando píxel Rojo en (Fila 0, Columna 5)..." << std::endl;
                    std::cout << "[OK] Dirección de la raíz de la matriz: " << capa->raizMatriz << std::endl;
                    std::cout << "[OK] Identificador de control de la matriz: " << capa->raizMatriz->colorHex << std::endl;
                }

                std::cout << "---------------------------------------\n" << std::endl;
                break;
            }
                
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