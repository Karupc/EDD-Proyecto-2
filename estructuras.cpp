#include "estructuras.h"
#include <iostream>

// ==========================================
// IMPLEMENTACIÓN: ÁRBOL DE CAPAS
// ==========================================

ArbolCapas::ArbolCapas() {
    raiz = nullptr;
}

NodoArbolCapas* ArbolCapas::insertarRecursivo(NodoArbolCapas* nodo, int id) {
    if (nodo == nullptr) {
        NodoArbolCapas* nuevo = new NodoArbolCapas();
        nuevo->idCapa = id;
        
        // CONEXIÓN: Cada capa se inicializa con su propia Matriz Dispersa viva
        MatrizDispersa* nuevaMatriz = new MatrizDispersa();
        nuevo->raizMatriz = nuevaMatriz->getRaiz();
        
        nuevo->izquierdo = nullptr;
        nuevo->derecho = nullptr;
        return nuevo;
    }
    if (id < nodo->idCapa) {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, id);
    } else if (id > nodo->idCapa) {
        nodo->derecho = insertarRecursivo(nodo->derecho, id);
    }
    return nodo;
}

void ArbolCapas::insertar(int id) {
    raiz = insertarRecursivo(raiz, id);
}

NodoArbolCapas* ArbolCapas::buscarRecursivo(NodoArbolCapas* nodo, int id) {
    if (nodo == nullptr || nodo->idCapa == id) {
        return nodo;
    }
    if (id < nodo->idCapa) {
        return buscarRecursivo(nodo->izquierdo, id);
    }
    return buscarRecursivo(nodo->derecho, id);
}

NodoArbolCapas* ArbolCapas::buscar(int id) {
    return buscarRecursivo(raiz, id);
}

NodoArbolCapas* ArbolCapas::getRaiz() {
    return raiz;
}

// ==========================================
// IMPLEMENTACIÓN: LISTA CIRCULAR DE IMÁGENES
// ==========================================

ListaImagenes::ListaImagenes() {
    primero = nullptr;
}

void ListaImagenes::insertar(int id) {
    NodoImagen* nuevo = new NodoImagen();
    nuevo->idImagen = id;
    nuevo->infoCapas = nullptr;
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if (primero == nullptr) {
        primero = nuevo;
        primero->siguiente = primero;
        primero->anterior = primero;
        return;
    }

    if (id < primero->idImagen) {
        NodoImagen* ultimo = primero->anterior;
        nuevo->siguiente = primero;
        nuevo->anterior = ultimo;
        primero->anterior = nuevo;
        ultimo->siguiente = nuevo;
        primero = nuevo;
        return;
    }

    NodoImagen* actual = primero;
    while (actual->siguiente != primero && actual->siguiente->idImagen < id) {
        actual = actual->siguiente;
    }

    if (actual->idImagen == id || actual->siguiente->idImagen == id) {
        delete nuevo;
        return;
    }

    nuevo->siguiente = actual->siguiente;
    nuevo->anterior = actual;
    actual->siguiente->anterior = nuevo;
    actual->siguiente = nuevo;
}

NodoImagen* ListaImagenes::buscar(int id) {
    if (primero == nullptr) return nullptr;
    NodoImagen* actual = primero;
    do {
        if (actual->idImagen == id) return actual;
        actual = actual->siguiente;
    } while (actual != primero);
    return nullptr;
}

void ListaImagenes::agregarCapaAImagen(int idImagen, NodoArbolCapas* direccionCapa) {
    NodoImagen* img = buscar(idImagen);
    if (img == nullptr || direccionCapa == nullptr) return;

    NodoListaCapas* nuevoNodoCapa = new NodoListaCapas();
    nuevoNodoCapa->capaApuntada = direccionCapa;
    nuevoNodoCapa->siguiente = nullptr;

    if (img->infoCapas == nullptr) {
        img->infoCapas = nuevoNodoCapa;
    } else {
        NodoListaCapas* aux = img->infoCapas;
        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoNodoCapa;
    }
}

NodoImagen* ListaImagenes::getPrimero() {
    return primero;
}

// ==========================================
// IMPLEMENTACIÓN: ÁRBOL DE USUARIOS
// ==========================================

ArbolUsuarios::ArbolUsuarios() {
    raiz = nullptr;
}

NodoUsuario* ArbolUsuarios::insertarRecursivo(NodoUsuario* nodo, std::string nombreUsuario) {
    if (nodo == nullptr) {
        NodoUsuario* nuevo = new NodoUsuario();
        nuevo->nombre = nombreUsuario;
        nuevo->imagenesPoseidas = nullptr;
        nuevo->izquierdo = nullptr;
        nuevo->derecho = nullptr;
        return nuevo;
    }
    if (nombreUsuario < nodo->nombre) {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, nombreUsuario);
    } else if (nombreUsuario > nodo->nombre) {
        nodo->derecho = insertarRecursivo(nodo->derecho, nombreUsuario);
    }
    return nodo;
}

void ArbolUsuarios::insertar(std::string nombreUsuario) {
    raiz = insertarRecursivo(raiz, nombreUsuario);
}

NodoUsuario* ArbolUsuarios::buscar(std::string nombreUsuario) {
    if (raiz == nullptr) return nullptr;
    NodoUsuario* actual = raiz;
    while (actual != nullptr) {
        if (nombreUsuario == actual->nombre) return actual;
        if (nombreUsuario < actual->nombre) actual = actual->izquierdo;
        else actual = actual->derecho;
    }
    return nullptr;
}

void ArbolUsuarios::agregarImagenAUsuario(std::string nombreUsuario, int idImagen) {
    NodoUsuario* user = buscar(nombreUsuario);
    if (user == nullptr) return;

    NodoListaImagenesUsuario* nuevoItem = new NodoListaImagenesUsuario();
    nuevoItem->idImagen = idImagen;
    nuevoItem->siguiente = nullptr;

    if (user->imagenesPoseidas == nullptr) {
        user->imagenesPoseidas = nuevoItem;
    } else {
        NodoListaImagenesUsuario* aux = user->imagenesPoseidas;
        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoItem;
    }
}

NodoUsuario* ArbolUsuarios::getRaiz() {
    return raiz;
}

// ==========================================
// IMPLEMENTACIÓN: MATRIZ DISPERSA
// ==========================================

MatrizDispersa::MatrizDispersa() {
    raiz = new NodoMatriz();
    raiz->fila = -1;
    raiz->columna = -1;
    raiz->colorHex = "RAIZ";
    raiz->arriba = raiz;
    raiz->abajo = raiz;
    raiz->izquierda = raiz;
    raiz->derecha = raiz;
}

NodoMatriz* MatrizDispersa::crearCabeceraFila(int fila) {
    NodoMatriz* actual = raiz;
    
    while (actual->abajo != raiz && actual->abajo->fila < fila) {
        actual = actual->abajo;
    }

    if (actual->abajo != raiz && actual->abajo->fila == fila) {
        return actual->abajo;
    }

    NodoMatriz* nuevaFila = new NodoMatriz();
    nuevaFila->fila = fila;
    nuevaFila->columna = -1;
    nuevaFila->colorHex = "FILA";
    
    nuevaFila->abajo = actual->abajo;
    actual->abajo->arriba = nuevaFila;
    nuevaFila->arriba = actual;
    actual->abajo = nuevaFila;

    nuevaFila->izquierda = nuevaFila;
    nuevaFila->derecha = nuevaFila;

    return nuevaFila;
}

NodoMatriz* MatrizDispersa::crearCabeceraColumna(int columna) {
    NodoMatriz* actual = raiz;

    while (actual->derecha != raiz && actual->derecha->columna < columna) {
        actual = actual->derecha;
    }

    if (actual->derecha != raiz && actual->derecha->columna == columna) {
        return actual->derecha;
    }

    NodoMatriz* nuevaColumna = new NodoMatriz();
    nuevaColumna->fila = -1;
    nuevaColumna->columna = columna;
    nuevaColumna->colorHex = "COL";

    nuevaColumna->derecha = actual->derecha;
    actual->derecha->izquierda = nuevaColumna;
    nuevaColumna->izquierda = actual;
    actual->derecha = nuevaColumna;

    nuevaColumna->arriba = nuevaColumna;
    nuevaColumna->abajo = nuevaColumna;

    return nuevaColumna;
}

void MatrizDispersa::insertarPixel(int fila, int columna, std::string colorHex) {
    NodoMatriz* cabFila = crearCabeceraFila(fila);
    NodoMatriz* cabCol = crearCabeceraColumna(columna);

    NodoMatriz* nuevoPixel = new NodoMatriz();
    nuevoPixel->fila = fila;
    nuevoPixel->columna = columna;
    nuevoPixel->colorHex = colorHex;

    NodoMatriz* auxHoriz = cabFila;
    while (auxHoriz->derecha != cabFila && auxHoriz->derecha->columna < columna) {
        auxHoriz = auxHoriz->derecha;
    }
    
    if (auxHoriz->derecha != cabFila && auxHoriz->derecha->columna == columna) {
        auxHoriz->derecha->colorHex = colorHex;
        delete nuevoPixel;
        return;
    }

    nuevoPixel->derecha = auxHoriz->derecha;
    auxHoriz->derecha->izquierda = nuevoPixel;
    nuevoPixel->izquierda = auxHoriz;
    auxHoriz->derecha = nuevoPixel;

    NodoMatriz* auxVert = cabCol;
    while (auxVert->abajo != cabCol && auxVert->abajo->fila < fila) {
        auxVert = auxVert->abajo;
    }

    nuevoPixel->abajo = auxVert->abajo;
    auxVert->abajo->arriba = nuevoPixel;
    nuevoPixel->arriba = auxVert;
    auxVert->abajo = nuevoPixel;
}

NodoMatriz* MatrizDispersa::getRaiz() {
    return raiz;
}