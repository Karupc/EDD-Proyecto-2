#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include <fstream>

// ============================================================
//  NODOS DE LA MATRIZ DISPERSA (lista circular doble enlazada)
// ============================================================
struct NodoMatriz {
    int fila;
    int columna;
    std::string colorHex;

    NodoMatriz* arriba;
    NodoMatriz* abajo;
    NodoMatriz* izquierda;
    NodoMatriz* derecha;
};

// ============================================================
//  NODO DEL ÁRBOL BINARIO DE BÚSQUEDA DE CAPAS
// ============================================================
struct NodoArbolCapas {
    int idCapa;
    NodoMatriz* raizMatriz;   // Apunta a la raíz de la matriz dispersa

    NodoArbolCapas* izquierdo;
    NodoArbolCapas* derecho;
};

// ============================================================
//  NODO DE LA LISTA DE CAPAS DE CADA IMAGEN
//  (apuntador al nodo del árbol, NO copia)
// ============================================================
struct NodoListaCapas {
    NodoArbolCapas* capaApuntada;
    NodoListaCapas* siguiente;
};

// ============================================================
//  NODO DE LA LISTA CIRCULAR DOBLE ENLAZADA DE IMÁGENES
// ============================================================
struct NodoImagen {
    int idImagen;
    NodoListaCapas* infoCapas;

    NodoImagen* siguiente;
    NodoImagen* anterior;
};

// ============================================================
//  NODO DE LA LISTA DE IMÁGENES DE UN USUARIO
// ============================================================
struct NodoListaImagenesUsuario {
    int idImagen;
    NodoListaImagenesUsuario* siguiente;
};

// ============================================================
//  NODO DEL ÁRBOL BINARIO DE BÚSQUEDA DE USUARIOS
// ============================================================
struct NodoUsuario {
    std::string nombre;
    NodoListaImagenesUsuario* imagenesPoseidas;

    NodoUsuario* izquierdo;
    NodoUsuario* derecho;
};

// ============================================================
//  CLASE MATRIZ DISPERSA
// ============================================================
class MatrizDispersa {
private:
    NodoMatriz* raiz;
    NodoMatriz* crearCabeceraFila(int fila);
    NodoMatriz* crearCabeceraColumna(int columna);

public:
    MatrizDispersa();
    void insertarPixel(int fila, int columna, std::string colorHex);
    NodoMatriz* getRaiz();
    void generarReporte(int idCapa);
    void setRaizManual(NodoMatriz* nuevaRaiz);
};

// ============================================================
//  CLASE ÁRBOL DE CAPAS (ABB)
// ============================================================
class ArbolCapas {
private:
    NodoArbolCapas* raiz;
    NodoArbolCapas* insertarRecursivo(NodoArbolCapas* nodo, int id);
    NodoArbolCapas* buscarRecursivo(NodoArbolCapas* nodo, int id);
    void generarDotRecursivo(NodoArbolCapas* nodo, std::ofstream& archivo);
    // Recorridos para generación de imagen
    void recorridoInorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado);
    void recorridoPreorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado);
    void recorridoPostorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado);

public:
    ArbolCapas();
    void insertar(int id);
    NodoArbolCapas* buscar(int id);
    NodoArbolCapas* getRaiz();
    void generarReporte();
    // Genera imagen PNG superponiendo capas por recorrido
    void generarImagenPorRecorrido(int numCapas, int tipoRecorrido);
};

// ============================================================
//  CLASE LISTA CIRCULAR DOBLE ENLAZADA DE IMÁGENES
// ============================================================
class ListaImagenes {
private:
    NodoImagen* primero;

public:
    ListaImagenes();
    void insertar(int id);
    bool eliminar(int id);
    NodoImagen* buscar(int id);
    void agregarCapaAImagen(int idImagen, NodoArbolCapas* direccionCapa);
    NodoImagen* getPrimero();
    void generarReporte();
    // Genera imagen PNG a partir de las capas de una imagen
    void generarImagenPorId(int idImagen);
    // Genera reporte combinado imagen+árbol de capas (ilustración 7)
    void generarReporteImagenYArbol(int idImagen, NodoArbolCapas* raizArbol);
};

// ============================================================
//  CLASE ÁRBOL DE USUARIOS (ABB)
// ============================================================
class ArbolUsuarios {
private:
    NodoUsuario* raiz;
    NodoUsuario* insertarRecursivo(NodoUsuario* nodo, std::string nombreUsuario);
    NodoUsuario* eliminarRecursivo(NodoUsuario* nodo, std::string nombreUsuario);
    NodoUsuario* minimoNodo(NodoUsuario* nodo);
    void generarDotRecursivo(NodoUsuario* nodo, std::ofstream& archivo);
    void listarNombresRecursivo(NodoUsuario* nodo);

public:
    ArbolUsuarios();
    void insertar(std::string nombreUsuario);
    bool eliminar(std::string nombreUsuario);
    NodoUsuario* buscar(std::string nombreUsuario);
    void agregarImagenAUsuario(std::string nombreUsuario, int idImagen);
    void eliminarImagenDeUsuario(std::string nombreUsuario, int idImagen);
    NodoUsuario* getRaiz();
    void generarReporte();
    void listarUsuarios();
};

void superponerCapasYGenerar(NodoListaCapas* listaCapas, const std::string& nombreSalida);
void generarImagenDesdeCapa(NodoArbolCapas* capa, const std::string& nombreSalida);

#endif