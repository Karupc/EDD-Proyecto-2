#include "sistema.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

// Elimina espacios, \r y \n de un string
static std::string limpiar(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), ' '),  s.end());
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    return s;
}

void CargaMasiva::cargarCapas(std::string ruta, ArbolCapas& arbol) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo abrir el archivo de capas: " << ruta << std::endl;
        return;
    }

    std::string linea;
    int idCapaActual = -1;

    while (std::getline(archivo, linea)) {
        linea = limpiar(linea);
        if (linea.empty()) continue;
        if (linea == "}") { idCapaActual = -1; continue; }

        // ¿Termina en '{'? → es la línea del id
        if (linea.back() == '{') {
            std::string sId = linea.substr(0, linea.size() - 1);
            sId = limpiar(sId);
            if (!sId.empty()) {
                try {
                    idCapaActual = std::stoi(sId);
                    arbol.insertar(idCapaActual);
                } catch (...) {
                    std::cout << "[Advertencia] ID de capa no válido: " << sId << std::endl;
                }
            }
            continue;
        }

        // ¿Contiene comas? → es un pixel  fila,columna,#color;
        if (idCapaActual != -1 && linea.find(',') != std::string::npos) {
            // Quitar punto y coma final
            if (!linea.empty() && linea.back() == ';') linea.pop_back();

            std::stringstream ss(linea);
            std::string sFila, sCol, colorHex;

            if (std::getline(ss, sFila, ',') &&
                std::getline(ss, sCol,  ',') &&
                std::getline(ss, colorHex, ',')) {

                sFila    = limpiar(sFila);
                sCol     = limpiar(sCol);
                colorHex = limpiar(colorHex);

                try {
                    int f = std::stoi(sFila);
                    int c = std::stoi(sCol);

                    NodoArbolCapas* capa = arbol.buscar(idCapaActual);
                    if (capa != nullptr) {
                        // Inicializar raíz de la matriz dispersa si aún no existe
                        if (capa->raizMatriz == nullptr) {
                            NodoMatriz* r = new NodoMatriz();
                            r->fila     = -1;
                            r->columna  = -1;
                            r->colorHex = "RAIZ";
                            r->arriba   = r;
                            r->abajo    = r;
                            r->izquierda = r;
                            r->derecha   = r;
                            capa->raizMatriz = r;
                        }

                        // Usar MatrizDispersa con la raíz existente
                        MatrizDispersa mat;
                        mat.setRaizManual(capa->raizMatriz);
                        mat.insertarPixel(f, c, colorHex);
                    }
                } catch (...) {
                    std::cout << "[Advertencia] Línea de pixel no válida: " << linea << std::endl;
                }
            }
        }
    }

    archivo.close();
    std::cout << "[OK] Archivo de capas cargado: " << ruta << std::endl;
}

void CargaMasiva::cargarImagenes(std::string ruta, ListaImagenes& lista, ArbolCapas& arbol) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo abrir el archivo de imágenes: " << ruta << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        linea = limpiar(linea);
        if (linea.empty()) continue;

        size_t llaveAbre  = linea.find('{');
        size_t llaveCierra = linea.find('}');

        if (llaveAbre == std::string::npos || llaveCierra == std::string::npos) continue;

        std::string sId = linea.substr(0, llaveAbre);
        sId = limpiar(sId);
        if (sId.empty()) continue;

        int idImg = -1;
        try { idImg = std::stoi(sId); } catch (...) { continue; }

        lista.insertar(idImg);

        std::string capasTexto = linea.substr(llaveAbre + 1, llaveCierra - llaveAbre - 1);
        capasTexto = limpiar(capasTexto);
        if (capasTexto.empty()) continue;

        std::stringstream ss(capasTexto);
        std::string token;
        while (std::getline(ss, token, ',')) {
            token = limpiar(token);
            if (!token.empty()) {
                try {
                    int idCapa = std::stoi(token);
                    NodoArbolCapas* capNodo = arbol.buscar(idCapa);
                    if (capNodo != nullptr) {
                        lista.agregarCapaAImagen(idImg, capNodo);
                    } else {
                        std::cout << "[Advertencia] Capa " << idCapa
                                  << " no existe, ignorada en imagen " << idImg << std::endl;
                    }
                } catch (...) {}
            }
        }
    }

    archivo.close();
    std::cout << "[OK] Archivo de imágenes cargado: " << ruta << std::endl;
}

void CargaMasiva::cargarUsuarios(std::string ruta, ArbolUsuarios& arbolUsr) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo abrir el archivo de usuarios: " << ruta << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        // No limpiar espacios del nombre completo, sólo \r y fin de línea
        linea.erase(std::remove(linea.begin(), linea.end(), '\r'), linea.end());
        if (linea.empty()) continue;

        size_t dosPuntos = linea.find(':');
        size_t puntoComa = linea.find(';');

        if (dosPuntos == std::string::npos || puntoComa == std::string::npos) continue;

        std::string nombre = linea.substr(0, dosPuntos);
        if (nombre.empty()) continue;

        arbolUsr.insertar(nombre);

        std::string imgsTexto = linea.substr(dosPuntos + 1, puntoComa - dosPuntos - 1);
        imgsTexto = limpiar(imgsTexto);
        if (imgsTexto.empty()) continue;

        std::stringstream ss(imgsTexto);
        std::string token;
        while (std::getline(ss, token, ',')) {
            token = limpiar(token);
            if (!token.empty()) {
                try {
                    int idImg = std::stoi(token);
                    arbolUsr.agregarImagenAUsuario(nombre, idImg);
                } catch (...) {}
            }
        }
    }

    archivo.close();
    std::cout << "[OK] Archivo de usuarios cargado: " << ruta << std::endl;
}