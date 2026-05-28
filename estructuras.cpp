#include "estructuras.h"
#include <iostream>
#include <fstream>

ArbolCapas::ArbolCapas() {
    raiz = nullptr;
}

NodoArbolCapas* ArbolCapas::insertarRecursivo(NodoArbolCapas* nodo, int id) {
    if (nodo == nullptr) {
        NodoArbolCapas* nuevo = new NodoArbolCapas();
        nuevo->idCapa = id;
        
        // Corrección de persistencia de la matriz
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


void ArbolUsuarios::generarDotRecursivo(NodoUsuario* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;

    archivo << "    \"" << nodo->nombre << "\" [label=\"" << nodo->nombre << "\" shape=box];\n";

    if (nodo->izquierdo != nullptr) {
        archivo << "    \"" << nodo->nombre << "\" -> \"" << nodo->izquierdo->nombre << "\" [label=\"izq\"];\n";
        generarDotRecursivo(nodo->izquierdo, archivo);
    }

    if (nodo->derecho != nullptr) {
        archivo << "    \"" << nodo->nombre << "\" -> \"" << nodo->derecho->nombre << "\" [label=\"der\"];\n";
        generarDotRecursivo(nodo->derecho, archivo);
    }
}

void ArbolUsuarios::generarReporte() {
    std::ofstream archivo("reporte_usuarios.dot");
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte." << std::endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "    node [fontname=\"Arial\", shape=box];\n";
    archivo << "    label=\"REPORTE ARBOL DE USUARIOS\";\n";

    if (raiz == nullptr) {
        // Si está vacío, le metemos nodos de prueba para ver si genera el archivo
        archivo << "    \"Usuario_Prueba_1\" -> \"Usuario_Prueba_2\";\n";
        archivo << "    \"Usuario_Prueba_1\" -> \"Usuario_Prueba_3\";\n";
    } else {
        generarDotRecursivo(raiz, archivo);
    }

    archivo << "}\n";
    
    archivo.flush(); // <-- CRÍTICO: Obliga a Windows a escribir el archivo YA.
    archivo.close();
    
    std::cout << "[OK] Archivo 'reporte_usuarios.dot' generado con exito." << std::endl;
}

void ArbolCapas::generarDotRecursivo(NodoArbolCapas* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;

    // Nodo actual (Guardando los IDs de las capas)
    archivo << "    " << nodo->idCapa << " [label=\"Capa " << nodo->idCapa << "\" shape=ellipse];\n";

    if (nodo->izquierdo != nullptr) {
        archivo << "    " << nodo->idCapa << " -> " << nodo->izquierdo->idCapa << " [label=\"izq\"];\n";
        generarDotRecursivo(nodo->izquierdo, archivo);
    }

    if (nodo->derecho != nullptr) {
        archivo << "    " << nodo->idCapa << " -> " << nodo->derecho->idCapa << " [label=\"der\"];\n";
        generarDotRecursivo(nodo->derecho, archivo);
    }
}

void ArbolCapas::generarReporte() {
    std::ofstream archivo("reporte_capas.dot");
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte de capas." << std::endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "    node [fontname=\"Arial\"];\n";
    archivo << "    label=\"REPORTE ARBOL DE CAPAS\";\n";

    if (raiz == nullptr) {
        archivo << "    \"Arbol de Capas Vacio\" [shape=none];\n";
    } else {
        generarDotRecursivo(raiz, archivo);
    }

    archivo << "}\n";
    archivo.flush();
    archivo.close();
    std::cout << "[OK] Archivo 'reporte_capas.dot' generado con exito." << std::endl;
}

void ListaImagenes::generarReporte() {
    std::ofstream archivo("reporte_imagenes.dot");
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte de imagenes." << std::endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "    rankdir=LR;\n"; // Hace que la lista se dibuje de izquierda a derecha
    archivo << "    node [fontname=\"Arial\", shape=box];\n";
    archivo << "    label=\"REPORTE LISTA DOBLE CIRCULAR DE IMAGENES\";\n";

    if (primero == nullptr) {
        archivo << "    \"Lista de Imagenes Vacia\" [shape=none];\n";
    } else {
        NodoImagen* actual = primero; // Nota: Si tu struct exacto se llama NodoImagen, cámbialo a NodoImagen*
        
        // Recorrer para declarar los nodos y sus enlaces siguientes
        do {
            archivo << "    \"Img_" << actual->idImagen << "\" [label=\"Imagen " << actual->idImagen << "\"];\n";
            
            // Flecha al siguiente
            archivo << "    \"Img_" << actual->idImagen << "\" -> \"Img_" << actual->siguiente->idImagen << "\" [constraint=true, label=\"sig\"];\n";
            
            // Flecha al anterior
            archivo << "    \"Img_" << actual->idImagen << "\" -> \"Img_" << actual->anterior->idImagen << "\" [constraint=true, label=\"ant\"];\n";
            
            actual = actual->siguiente;
        } while (actual != primero);
    }

    archivo << "}\n";
    archivo.flush();
    archivo.close();
    std::cout << "[OK] Archivo 'reporte_imagenes.dot' generado con exito." << std::endl;
}

void MatrizDispersa::generarReporte(int idCapa) {
    std::string nombreArchivo = "reporte_matriz_capa_" + std::to_string(idCapa) + ".dot";
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo de la matriz." << std::endl;
        return;
    }

    // Cabecera estándar de Graphviz para matrices ortogonales
    archivo << "digraph G {\n";
    archivo << "    node [fontname=\"Arial\", shape=box, width=0.7, height=0.7];\n";
    archivo << "    label=\"REPORTE MATRIZ DISPERSA - CAPA " << idCapa << "\";\n";
    archivo << "    rankdir=TB;\n";
    archivo << "    nodesep=0.4;\n";
    archivo << "    ranksep=0.4;\n";

    if (raiz->abajo == raiz && raiz->derecha == raiz) {
        archivo << "    \"Matriz Vacia\" [shape=none];\n";
        archivo << "}\n";
        archivo.close();
        return;
    }

    // 1. Declaración y alineación del Nodo RAÍZ con las Columnas
    archivo << "    N_R [label=\"RAIZ\", group=0, style=filled, fillcolor=lightgray];\n";

    // Declarar las cabeceras de columnas en el mismo nivel (rank=same) que la raíz
    NodoMatriz* col = raiz->derecha;
    archivo << "    { rank=same; N_R; ";
    while (col != raiz) {
        archivo << "N_C" << col->columna << "; ";
        col = col->derecha;
    }
    archivo << "}\n";

    // Estilo de los nodos de columnas
    col = raiz->derecha;
    while (col != raiz) {
        archivo << "    N_C" << col->columna << " [label=\"C" << col->columna << "\", group=" << col->columna << ", style=filled, fillcolor=lightblue];\n";
        col = col->derecha;
    }

    // Enlaces horizontales de la fila de cabecera de columnas
    archivo << "    N_R -> N_C" << raiz->derecha->columna << " [dir=both];\n";
    col = raiz->derecha;
    while (col->derecha != raiz) {
        archivo << "    N_C" << col->columna << " -> N_C" << col->derecha->columna << " [dir=both];\n";
        col = col->derecha;
    }

    // 2. Recorrido por Filas y creación de Nodos Internos
    NodoMatriz* fila = raiz->abajo;
    while (fila != raiz) {
        // Definir nodo de la cabecera de fila
        archivo << "    N_F" << fila->fila << " [label=\"F" << fila->fila << "\", group=0, style=filled, fillcolor=lightpink];\n";
        
        // Alinear la cabecera de fila con todos sus nodos internos horizontalmente
        archivo << "    { rank=same; N_F" << fila->fila << "; ";
        NodoMatriz* pxl = fila->derecha;
        while (pxl != fila) {
            archivo << "N_F" << pxl->fila << "_C" << pxl->columna << "; ";
            pxl = pxl->derecha;
        }
        archivo << "}\n";

        // Imprimir las propiedades de los píxeles internos (Corregido el error de comillas y punto y coma)
        pxl = fila->derecha;
        while (pxl != fila) {
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna 
                    << " [label=\"" << pxl->colorHex << "\", group=" << pxl->columna 
                    << ", style=filled, fillcolor=\"" << pxl->colorHex << "\", fontcolor=white];\n";
            pxl = pxl->derecha;
        }

        // Enlaces horizontales de los nodos internos de esta fila
        archivo << "    N_F" << fila->fila << " -> N_F" << fila->fila << "_C" << fila->derecha->columna << " [dir=both];\n";
        pxl = fila->derecha;
        while (pxl->derecha != fila) {
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna << " -> N_F" << pxl->derecha->fila << "_C" << pxl->derecha->columna << " [dir=both];\n";
            pxl = pxl->derecha;
        }
        // Cerrar el ciclo horizontal apuntando de vuelta a la cabecera (Lista circular)
        archivo << "    N_F" << pxl->fila << "_C" << pxl->columna << " -> N_F" << fila->fila << " [dir=both];\n";

        fila = fila->abajo;
    }

    // Enlaces verticales de la columna de cabecera de filas
    archivo << "    N_R -> N_F" << raiz->abajo->fila << " [dir=both];\n";
    fila = raiz->abajo;
    while (fila->abajo != raiz) {
        archivo << "    N_F" << fila->fila << " -> N_F" << fila->abajo->fila << " [dir=both];\n";
        fila = fila->abajo;
    }

    // 3. Enlaces verticales de los nodos internos (REESTRUCTURADO Y CORREGIDO COMPLETAMENTE)
    col = raiz->derecha;
    while (col != raiz) {
        NodoMatriz* pxl = col->abajo;
        if (pxl != col) {
            // Conexión desde la cabecera de columna al primer elemento hacia abajo
            archivo << "    N_C" << col->columna << " -> N_F" << pxl->fila << "_C" << pxl->columna << " [dir=both];\n";
            while (pxl->abajo != col) {
                // Conexión entre nodos internos de la misma columna
                archivo << "    N_F" << pxl->fila << "_C" << pxl->columna << " -> N_F" << pxl->abajo->fila << "_C" << pxl->abajo->columna << " [dir=both];\n";
                pxl = pxl->abajo;
            }
            // Cierre del ciclo vertical hacia la cabecera de columna
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna << " -> N_C" << col->columna << " [dir=both];\n";
        }
        col = col->derecha;
    }

    archivo << "}\n";
    archivo.flush();
    archivo.close();
    std::cout << "[OK] Reporte de matriz de capa generado limpiamente." << std::endl;
}

void MatrizDispersa::setRaizManual(NodoMatriz* nuevaRaiz) {
    if (nuevaRaiz != nullptr) {
        this->raiz = nuevaRaiz;
    }
}