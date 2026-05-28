#include <iostream>
#include <string>
#include <limits>
#include "estructuras.h"
#include "sistema.h"

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int leerEntero(const std::string& mensaje) {
    int valor;
    std::cout << mensaje;
    while (!(std::cin >> valor)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[Error] Ingresa un número válido. " << mensaje;
    }
    limpiarBuffer();
    return valor;
}

std::string leerTexto(const std::string& mensaje) {
    std::string valor;
    std::cout << mensaje;
    std::getline(std::cin, valor);
    return valor;
}

void menuCargaMasiva(ArbolCapas& capas, ListaImagenes& imagenes, ArbolUsuarios& usuarios) {
    CargaMasiva lector;

    std::cout << "\n--- CARGA MASIVA DE DATOS ---" << std::endl;
    std::cout << "Nota: el orden debe ser 1) Capas  2) Imágenes  3) Usuarios" << std::endl;

    std::string arcCapas = leerTexto("Archivo de Capas (.cap) [Enter para omitir]: ");
    if (!arcCapas.empty()) lector.cargarCapas(arcCapas, capas);

    std::string arcImagenes = leerTexto("Archivo de Imágenes (.im) [Enter para omitir]: ");
    if (!arcImagenes.empty()) lector.cargarImagenes(arcImagenes, imagenes, capas);

    std::string arcUsuarios = leerTexto("Archivo de Usuarios (.usr) [Enter para omitir]: ");
    if (!arcUsuarios.empty()) lector.cargarUsuarios(arcUsuarios, usuarios);
}

void menuGeneracion(ArbolCapas& capas, ListaImagenes& imagenes, ArbolUsuarios& usuarios) {
    int op = 0;
    do {
        std::cout << "\n--- GENERACIÓN DE IMÁGENES ---" << std::endl;
        std::cout << "1. Por recorrido limitado (Preorden / Inorden / Postorden)" << std::endl;
        std::cout << "2. Por lista de imágenes (ID de imagen)" << std::endl;
        std::cout << "3. Por capa (ID de capa)" << std::endl;
        std::cout << "4. Por usuario" << std::endl;
        std::cout << "0. Volver" << std::endl;
        op = leerEntero("Opción: ");

        if (op == 1) {
            // ---------- Por recorrido limitado ----------
            int numCapas = leerEntero("Número de capas a utilizar: ");
            std::cout << "Tipo de recorrido:" << std::endl;
            std::cout << "  1. Inorden" << std::endl;
            std::cout << "  2. Preorden" << std::endl;
            std::cout << "  3. Postorden" << std::endl;
            int tipo = leerEntero("Opción: ");
            if (tipo < 1 || tipo > 3) {
                std::cout << "[Error] Tipo inválido." << std::endl;
            } else {
                capas.generarImagenPorRecorrido(numCapas, tipo);
            }

        } else if (op == 2) {
            // ---------- Por lista de imágenes ----------
            int idImg = leerEntero("ID de la imagen a generar: ");
            imagenes.generarImagenPorId(idImg);

        } else if (op == 3) {
            // ---------- Por capa ----------
            int idCapa = leerEntero("ID de la capa a generar: ");
            NodoArbolCapas* nCapa = capas.buscar(idCapa);
            if (nCapa == nullptr) {
                std::cout << "[Error] Capa con ID " << idCapa << " no encontrada." << std::endl;
            } else {
                std::string salida = "imagen_capa_" + std::to_string(idCapa);
                generarImagenDesdeCapa(nCapa, salida);
            }

        } else if (op == 4) {
            // ---------- Por usuario ----------
            if (usuarios.getRaiz() == nullptr) {
                std::cout << "[!] No hay usuarios registrados." << std::endl;
            } else {
                std::cout << "Usuarios disponibles:" << std::endl;
                usuarios.listarUsuarios();
                std::string nombre = leerTexto("Nombre del usuario: ");
                NodoUsuario* usr = usuarios.buscar(nombre);
                if (usr == nullptr) {
                    std::cout << "[Error] Usuario '" << nombre << "' no encontrado." << std::endl;
                } else {
                    // Mostrar imágenes del usuario
                    NodoListaImagenesUsuario* li = usr->imagenesPoseidas;
                    if (li == nullptr) {
                        std::cout << "[!] Este usuario no tiene imágenes registradas." << std::endl;
                    } else {
                        std::cout << "Imágenes del usuario " << nombre << ": ";
                        while (li != nullptr) {
                            std::cout << li->idImagen << " ";
                            li = li->siguiente;
                        }
                        std::cout << std::endl;
                        int idImg = leerEntero("ID de la imagen a generar: ");
                        imagenes.generarImagenPorId(idImg);
                    }
                }
            }
        }
    } while (op != 0);
}

void menuCrudUsuarios(ArbolUsuarios& usuarios) {
    int op = 0;
    do {
        std::cout << "\n--- CRUD USUARIOS ---" << std::endl;
        std::cout << "1. Agregar usuario" << std::endl;
        std::cout << "2. Eliminar usuario" << std::endl;
        std::cout << "3. Modificar usuario (cambiar nombre)" << std::endl;
        std::cout << "4. Listar usuarios" << std::endl;
        std::cout << "0. Volver" << std::endl;
        op = leerEntero("Opción: ");

        if (op == 1) {
            std::string nombre = leerTexto("Nombre del nuevo usuario: ");
            if (nombre.empty()) { std::cout << "[Error] El nombre no puede estar vacío." << std::endl; continue; }
            if (usuarios.buscar(nombre) != nullptr) {
                std::cout << "[Error] Ya existe un usuario con ese nombre." << std::endl;
            } else {
                usuarios.insertar(nombre);
                std::cout << "[OK] Usuario '" << nombre << "' agregado." << std::endl;
            }

        } else if (op == 2) {
            std::cout << "Usuarios actuales:" << std::endl;
            usuarios.listarUsuarios();
            std::string nombre = leerTexto("Nombre del usuario a eliminar: ");
            if (usuarios.eliminar(nombre)) {
                std::cout << "[OK] Usuario '" << nombre << "' eliminado." << std::endl;
            } else {
                std::cout << "[Error] Usuario '" << nombre << "' no encontrado." << std::endl;
            }

        } else if (op == 3) {
            std::cout << "Usuarios actuales:" << std::endl;
            usuarios.listarUsuarios();
            std::string nombreActual = leerTexto("Nombre actual del usuario: ");
            NodoUsuario* usr = usuarios.buscar(nombreActual);
            if (usr == nullptr) {
                std::cout << "[Error] Usuario '" << nombreActual << "' no encontrado." << std::endl;
            } else {
                std::string nuevoNombre = leerTexto("Nuevo nombre: ");
                if (nuevoNombre.empty()) { std::cout << "[Error] El nombre no puede estar vacío." << std::endl; continue; }
                if (usuarios.buscar(nuevoNombre) != nullptr) {
                    std::cout << "[Error] Ya existe un usuario con ese nombre." << std::endl;
                } else {
                    // Guardar lista de imágenes, eliminar y reinsertar
                    NodoListaImagenesUsuario* imgs = usr->imagenesPoseidas;
                    usr->imagenesPoseidas = nullptr; // desconectar para que no se borre al eliminar
                    usuarios.eliminar(nombreActual);
                    usuarios.insertar(nuevoNombre);
                    NodoUsuario* nuevo = usuarios.buscar(nuevoNombre);
                    nuevo->imagenesPoseidas = imgs;
                    std::cout << "[OK] Usuario renombrado a '" << nuevoNombre << "'." << std::endl;
                }
            }

        } else if (op == 4) {
            std::cout << "Usuarios registrados:" << std::endl;
            usuarios.listarUsuarios();
        }
    } while (op != 0);
}

void menuCrudImagenes(ListaImagenes& imagenes, ArbolCapas& capas, ArbolUsuarios& usuarios) {
    int op = 0;
    do {
        std::cout << "\n--- CRUD IMÁGENES ---" << std::endl;
        std::cout << "1. Agregar imagen (con capas)" << std::endl;
        std::cout << "2. Eliminar imagen" << std::endl;
        std::cout << "0. Volver" << std::endl;
        op = leerEntero("Opción: ");

        if (op == 1) {
            // Seleccionar usuario
            std::cout << "Usuarios disponibles:" << std::endl;
            usuarios.listarUsuarios();
            std::string nombre = leerTexto("Nombre del usuario al que se asignará: ");
            NodoUsuario* usr = usuarios.buscar(nombre);
            if (usr == nullptr) {
                std::cout << "[Error] Usuario no encontrado." << std::endl; continue;
            }

            int idImg = leerEntero("ID de la nueva imagen: ");
            if (imagenes.buscar(idImg) != nullptr) {
                std::cout << "[Error] Ya existe una imagen con ID " << idImg << std::endl;
                continue;
            }

            imagenes.insertar(idImg);
            usuarios.agregarImagenAUsuario(nombre, idImg);

            // Agregar capas
            std::cout << "Ingrese IDs de capas a agregar (0 para terminar):" << std::endl;
            while (true) {
                int idCapa = leerEntero("  ID de capa: ");
                if (idCapa == 0) break;
                NodoArbolCapas* nc = capas.buscar(idCapa);
                if (nc == nullptr) {
                    std::cout << "  [Advertencia] Capa " << idCapa << " no existe, ignorada." << std::endl;
                } else {
                    imagenes.agregarCapaAImagen(idImg, nc);
                    std::cout << "  [OK] Capa " << idCapa << " agregada a imagen " << idImg << std::endl;
                }
            }
            std::cout << "[OK] Imagen " << idImg << " creada y asignada a " << nombre << std::endl;

        } else if (op == 2) {
            std::cout << "Usuarios disponibles:" << std::endl;
            usuarios.listarUsuarios();
            std::string nombre = leerTexto("Nombre del usuario dueño de la imagen: ");
            NodoUsuario* usr = usuarios.buscar(nombre);
            if (usr == nullptr) {
                std::cout << "[Error] Usuario no encontrado." << std::endl; continue;
            }

            NodoListaImagenesUsuario* li = usr->imagenesPoseidas;
            if (li == nullptr) {
                std::cout << "[!] Este usuario no tiene imágenes." << std::endl; continue;
            }
            std::cout << "Imágenes del usuario: ";
            while (li != nullptr) { std::cout << li->idImagen << " "; li = li->siguiente; }
            std::cout << std::endl;

            int idImg = leerEntero("ID de la imagen a eliminar: ");

            // Eliminar de la lista del usuario
            usuarios.eliminarImagenDeUsuario(nombre, idImg);
            // Eliminar de la lista circular global
            if (imagenes.eliminar(idImg)) {
                std::cout << "[OK] Imagen " << idImg << " eliminada." << std::endl;
            } else {
                std::cout << "[Error] Imagen " << idImg << " no encontrada en la lista global." << std::endl;
            }
        }
    } while (op != 0);
}

void menuReportes(ArbolCapas& capas, ListaImagenes& imagenes, ArbolUsuarios& usuarios) {
    int op = 0;
    do {
        std::cout << "\n--- ESTADO DE MEMORIA / REPORTES ---" << std::endl;
        std::cout << "1. Ver árbol de capas" << std::endl;
        std::cout << "2. Ver lista de imágenes (con capas de cada una)" << std::endl;
        std::cout << "3. Ver árbol de usuarios" << std::endl;
        std::cout << "4. Ver capa (Matriz Dispersa)" << std::endl;
        std::cout << "5. Ver imagen + árbol de capas combinado" << std::endl;
        std::cout << "0. Volver" << std::endl;
        op = leerEntero("Opción: ");

        if (op == 1) {
            capas.generarReporte();

        } else if (op == 2) {
            imagenes.generarReporte();

        } else if (op == 3) {
            usuarios.generarReporte();

        } else if (op == 4) {
            int idCapa = leerEntero("ID de la capa a visualizar: ");
            NodoArbolCapas* nCapa = capas.buscar(idCapa);
            if (nCapa == nullptr) {
                std::cout << "[Error] Capa " << idCapa << " no encontrada." << std::endl;
            } else {
                MatrizDispersa mat;
                if (nCapa->raizMatriz != nullptr) {
                    mat.setRaizManual(nCapa->raizMatriz);
                }
                mat.generarReporte(idCapa);
            }

        } else if (op == 5) {
            int idImg = leerEntero("ID de la imagen: ");
            imagenes.generarReporteImagenYArbol(idImg, capas.getRaiz());
        }
    } while (op != 0);
}

int main() {
    // UTF-8 en Windows (no afecta Linux/Mac)
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    ArbolCapas    sistemaCapas;
    ListaImagenes sistemaImagenes;
    ArbolUsuarios sistemaUsuarios;

    int opcion = -1;

    while (opcion != 0) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "    GENERADOR DE IMÁGENES POR CAPAS    " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "1. Carga Masiva de datos" << std::endl;
        std::cout << "2. Generación de imágenes" << std::endl;
        std::cout << "3. CRUD Usuarios" << std::endl;
        std::cout << "4. CRUD Imágenes" << std::endl;
        std::cout << "5. Estado de la memoria / Reportes" << std::endl;
        std::cout << "0. Salir" << std::endl;
        opcion = leerEntero("Seleccione una opción: ");

        switch (opcion) {
            case 1:
                menuCargaMasiva(sistemaCapas, sistemaImagenes, sistemaUsuarios);
                break;
            case 2:
                menuGeneracion(sistemaCapas, sistemaImagenes, sistemaUsuarios);
                break;
            case 3:
                menuCrudUsuarios(sistemaUsuarios);
                break;
            case 4:
                menuCrudImagenes(sistemaImagenes, sistemaCapas, sistemaUsuarios);
                break;
            case 5:
                menuReportes(sistemaCapas, sistemaImagenes, sistemaUsuarios);
                break;
            case 0:
                std::cout << "\n¡Hasta luego!\n" << std::endl;
                break;
            default:
                std::cout << "[Error] Opción inválida.\n" << std::endl;
        }
    }

    return 0;
}