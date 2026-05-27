#include "sistema.h"
#include <iostream>
#include <fstream>
#include <sstream>

void CargaMasiva::cargarCapas(std::string ruta, ArbolCapas& arbol) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de capas." << std::endl;
        return;
    }

    std::string linea;
    int idCapaActual = -1;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        if (linea.find(',') == std::string::npos) {
            try {
                idCapaActual = std::stoi(linea);
                arbol.insertar(idCapaActual);
            } catch (...) {
                continue;
            }
        } else {
            if (idCapaActual == -1) continue;

            std::stringstream ss(linea);
            std::string sFila, sCol, colorHex;

            if (std::getline(ss, sFila, ',') && 
                std::getline(ss, sCol, ',') && 
                std::getline(ss, colorHex, ',')) {
                
                NodoArbolCapas* capa = arbol.buscar(idCapaActual);
                if (capa != nullptr) {
                    int f = std::stoi(sFila);
                    int c = std::stoi(sCol);
                    
                    if (capa->raizMatriz == nullptr) {
                        capa->raizMatriz = new NodoMatriz();
                        capa->raizMatriz->fila = -1;
                        capa->raizMatriz->columna = -1;
                        capa->raizMatriz->colorHex = "RAIZ";
                        capa->raizMatriz->arriba = capa->raizMatriz;
                        capa->raizMatriz->abajo = capa->raizMatriz;
                        capa->raizMatriz->izquierda = capa->raizMatriz;
                        capa->raizMatriz->derecha = capa->raizMatriz;
                    }

                    NodoMatriz* nuevo = new NodoMatriz();
                    nuevo->fila = f;
                    nuevo->columna = c;
                    nuevo->colorHex = colorHex;

                    NodoMatriz* r = capa->raizMatriz;
                    nuevo->abajo = r->abajo;
                    r->abajo->arriba = nuevo;
                    r->abajo = nuevo;
                    nuevo->arriba = r;

                    nuevo->derecha = r->derecha;
                    r->derecha->izquierda = nuevo;
                    r->derecha = nuevo;
                    nuevo->izquierda = r;
                }
            }
        }
    }
    archivo.close();
    std::cout << "[OK] Archivo de capas cargado con exito." << std::endl;
}

void CargaMasiva::cargarImagenes(std::string ruta, ListaImagenes& lista, ArbolCapas& arbol) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de imagenes." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t llaveAbre = linea.find('{');
        size_t llaveCierra = linea.find('}');
        
        if (llaveAbre != std::string::npos && llaveCierra != std::string::npos) {
            std::string sId = linea.substr(0, llaveAbre);
            int idImg = std::stoi(sId);
            
            lista.insertar(idImg);
            
            std::string capasTexto = linea.substr(llaveAbre + 1, llaveCierra - llaveAbre - 1);
            if (capasTexto.empty()) continue;

            std::stringstream ss(capasTexto);
            std::string token;
            
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    int idCapa = std::stoi(token);
                    NodoArbolCapas* capNodo = arbol.buscar(idCapa);
                    if (capNodo != nullptr) {
                        lista.agregarCapaAImagen(idImg, capNodo);
                    }
                }
            }
        }
    }
    archivo.close();
    std::cout << "[OK] Archivo de imagenes cargado con exito." << std::endl;
}

void CargaMasiva::cargarUsuarios(std::string ruta, ArbolUsuarios& arbolUsr) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t dosPuntos = linea.find(':');
        size_t puntoComa = linea.find(';');

        if (dosPuntos != std::string::npos && puntoComa != std::string::npos) {
            std::string nombre = linea.substr(0, dosPuntos);
            arbolUsr.insertar(nombre);

            std::string imgsTexto = linea.substr(dosPuntos + 1, puntoComa - dosPuntos - 1);
            if (imgsTexto.empty()) continue;

            std::stringstream ss(imgsTexto);
            std::string token;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    int idImg = std::stoi(token);
                    arbolUsr.agregarImagenAUsuario(nombre, idImg);
                }
            }
        }
    }
    archivo.close();
    std::cout << "[OK] Archivo de usuarios cargado con exito." << std::endl;
}