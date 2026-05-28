#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>

struct NodoMatriz {
    int fila;
    int columna;
    std::string colorHex;
    
    NodoMatriz* arriba;
    NodoMatriz* abajo;
    NodoMatriz* izquierda;
    NodoMatriz* derecha;
};

struct NodoArbolCapas {
    int idCapa;
    NodoMatriz* raizMatriz; 
    
    NodoArbolCapas* izquierdo;
    NodoArbolCapas* derecho;
};

struct NodoListaCapas {
    NodoArbolCapas* capaApuntada; 
    NodoListaCapas* siguiente;
};

struct NodoImagen {
    int idImagen;
    NodoListaCapas* infoCapas; 
    
    NodoImagen* siguiente;
    NodoImagen* anterior;
};

struct NodoListaImagenesUsuario {
    int idImagen;
    NodoListaImagenesUsuario* siguiente;
};

struct NodoUsuario {
    std::string nombre;
    NodoListaImagenesUsuario* imagenesPoseidas; 
    
    NodoUsuario* izquierdo;
    NodoUsuario* derecho;
};

class MatrizDispersa {
private:
    NodoMatriz* raiz;
    NodoMatriz* crearCabeceraFila(int fila);
    NodoMatriz* crearCabeceraColumna(int columna);

public:
    MatrizDispersa();
    void insertarPixel(int fila, int columna, std::string colorHex);
    NodoMatriz* getRaiz();
};

class ArbolCapas {
private:
    NodoArbolCapas* raiz;
    NodoArbolCapas* insertarRecursivo(NodoArbolCapas* nodo, int id);
    NodoArbolCapas* buscarRecursivo(NodoArbolCapas* nodo, int id);
    void generarDotRecursivo(NodoArbolCapas* nodo, std::ofstream& archivo);

public:
    ArbolCapas();
    void insertar(int id);
    NodoArbolCapas* buscar(int id);
    NodoArbolCapas* getRaiz();
    void generarReporte();
};

class ListaImagenes {
private:
    NodoImagen* primero;

public:
    ListaImagenes();
    void insertar(int id);
    NodoImagen* buscar(int id);
    void agregarCapaAImagen(int idImagen, NodoArbolCapas* direccionCapa);
    NodoImagen* getPrimero();
    // --- AGREGA ESTA LÍNEA PÚBLICA ---
    void generarReporte();
};

class ArbolUsuarios {
private:
    NodoUsuario* raiz;
    NodoUsuario* insertarRecursivo(NodoUsuario* nodo, std::string nombreUsuario);
    // 1. AGREGA ESTA LÍNEA PRIVADA:
    void generarDotRecursivo(NodoUsuario* nodo, std::ofstream& archivo);

public:
    ArbolUsuarios();
    void insertar(std::string nombreUsuario);
    NodoUsuario* buscar(std::string nombreUsuario);
    void agregarImagenAUsuario(std::string nombreUsuario, int idImagen);
    NodoUsuario* getRaiz();
    // 2. AGREGA ESTA LÍNEA PÚBLICA:
    void generarReporte();
};

#endif