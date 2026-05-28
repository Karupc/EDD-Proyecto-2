#include "estructuras.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <functional>

// Superpone una lista de capas (en orden de inserción) y genera PNG.
// La última capa en la lista "tapa" a las anteriores en celdas compartidas.
void superponerCapasYGenerar(NodoListaCapas* listaCapas, const std::string& nombreSalida) {
    // Mapa (fila,col) -> color. Recorremos en orden; el último que escriba gana.
    std::map<std::pair<int,int>, std::string> pixeles;

    NodoListaCapas* nc = listaCapas;
    while (nc != nullptr) {
        NodoArbolCapas* capa = nc->capaApuntada;
        if (capa != nullptr && capa->raizMatriz != nullptr) {
            NodoMatriz* raizM = capa->raizMatriz;
            // Recorrer filas
            NodoMatriz* fila = raizM->abajo;
            while (fila != raizM) {
                NodoMatriz* pxl = fila->derecha;
                while (pxl != fila) {
                    pixeles[{pxl->fila, pxl->columna}] = pxl->colorHex;
                    pxl = pxl->derecha;
                }
                fila = fila->abajo;
            }
        }
        nc = nc->siguiente;
    }

    if (pixeles.empty()) {
        // Imagen vacía: un solo píxel negro
        pixeles[{0,0}] = "#000000";
    }

    // Determinar dimensiones
    int maxFila = 0, maxCol = 0;
    for (auto& p : pixeles) {
        if (p.first.first  > maxFila) maxFila = p.first.first;
        if (p.first.second > maxCol)  maxCol  = p.first.second;
    }

    int CELDA = 20; // px por celda

    std::string dotFile = nombreSalida + ".dot";
    std::ofstream dot(dotFile);
    dot << "graph G {\n";
    dot << "    bgcolor=white;\n";
    dot << "    node [shape=point, width=0, height=0, label=\"\"];\n";
    dot << "    graph [pad=0, nodesep=0, ranksep=0];\n";

    // Usamos una tabla HTML dentro de un nodo para dibujar los píxeles
    dot << "    imagen [shape=none, margin=0, label=<\n";
    dot << "    <TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"0\">\n";

    for (int f = 0; f <= maxFila; f++) {
        dot << "    <TR>";
        for (int c = 0; c <= maxCol; c++) {
            auto it = pixeles.find({f, c});
            std::string color = "white";
            if (it != pixeles.end()) {
                color = it->second;
                // Asegurar que empiece con #
                if (color[0] != '#') color = "#" + color;
            }
            dot << "<TD WIDTH=\"" << CELDA << "\" HEIGHT=\"" << CELDA
                << "\" BGCOLOR=\"" << color << "\"> </TD>";
        }
        dot << "</TR>\n";
    }
    dot << "    </TABLE>>];\n";
    dot << "}\n";
    dot.flush();
    dot.close();

    std::string cmd = "dot -Tpng \"" + dotFile + "\" -o \"" + nombreSalida + ".png\" 2>&1";
    int ret = system(cmd.c_str());
    if (ret == 0) {
        std::cout << "[OK] Imagen generada: " << nombreSalida << ".png" << std::endl;
    } else {
        std::cout << "[Error] Graphviz no pudo generar la imagen. Verifica que 'dot' esté instalado." << std::endl;
        std::cout << "        El archivo .dot fue creado: " << dotFile << std::endl;
    }
}

void generarImagenDesdeCapa(NodoArbolCapas* capa, const std::string& nombreSalida) {
    if (capa == nullptr) return;
    // Creamos una lista de capas temporal con sólo esta capa
    NodoListaCapas nodoTmp;
    nodoTmp.capaApuntada = capa;
    nodoTmp.siguiente = nullptr;
    superponerCapasYGenerar(&nodoTmp, nombreSalida);
}


MatrizDispersa::MatrizDispersa() {
    raiz = new NodoMatriz();
    raiz->fila    = -1;
    raiz->columna = -1;
    raiz->colorHex = "RAIZ";
    raiz->arriba    = raiz;
    raiz->abajo     = raiz;
    raiz->izquierda = raiz;
    raiz->derecha   = raiz;
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
    nuevaFila->fila    = fila;
    nuevaFila->columna = -1;
    nuevaFila->colorHex = "FILA";

    nuevaFila->abajo        = actual->abajo;
    actual->abajo->arriba   = nuevaFila;
    nuevaFila->arriba       = actual;
    actual->abajo           = nuevaFila;

    nuevaFila->izquierda = nuevaFila;
    nuevaFila->derecha   = nuevaFila;
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
    nuevaColumna->fila    = -1;
    nuevaColumna->columna = columna;
    nuevaColumna->colorHex = "COL";

    nuevaColumna->derecha          = actual->derecha;
    actual->derecha->izquierda     = nuevaColumna;
    nuevaColumna->izquierda        = actual;
    actual->derecha                = nuevaColumna;

    nuevaColumna->arriba = nuevaColumna;
    nuevaColumna->abajo  = nuevaColumna;
    return nuevaColumna;
}

void MatrizDispersa::insertarPixel(int fila, int columna, std::string colorHex) {
    // Limpiar espacios en el color
    colorHex.erase(std::remove(colorHex.begin(), colorHex.end(), ' '), colorHex.end());
    colorHex.erase(std::remove(colorHex.begin(), colorHex.end(), '\r'), colorHex.end());

    NodoMatriz* cabFila = crearCabeceraFila(fila);
    NodoMatriz* cabCol  = crearCabeceraColumna(columna);

    // ¿Ya existe el píxel?
    NodoMatriz* auxHoriz = cabFila;
    while (auxHoriz->derecha != cabFila && auxHoriz->derecha->columna < columna) {
        auxHoriz = auxHoriz->derecha;
    }
    if (auxHoriz->derecha != cabFila && auxHoriz->derecha->columna == columna) {
        auxHoriz->derecha->colorHex = colorHex; // actualizar color
        return;
    }

    NodoMatriz* nuevoPixel = new NodoMatriz();
    nuevoPixel->fila     = fila;
    nuevoPixel->columna  = columna;
    nuevoPixel->colorHex = colorHex;

    // Enlace horizontal
    nuevoPixel->derecha          = auxHoriz->derecha;
    auxHoriz->derecha->izquierda = nuevoPixel;
    nuevoPixel->izquierda        = auxHoriz;
    auxHoriz->derecha            = nuevoPixel;

    // Enlace vertical
    NodoMatriz* auxVert = cabCol;
    while (auxVert->abajo != cabCol && auxVert->abajo->fila < fila) {
        auxVert = auxVert->abajo;
    }
    nuevoPixel->abajo        = auxVert->abajo;
    auxVert->abajo->arriba   = nuevoPixel;
    nuevoPixel->arriba       = auxVert;
    auxVert->abajo           = nuevoPixel;
}

NodoMatriz* MatrizDispersa::getRaiz() {
    return raiz;
}

void MatrizDispersa::setRaizManual(NodoMatriz* nuevaRaiz) {
    if (nuevaRaiz != nullptr) {
        this->raiz = nuevaRaiz;
    }
}

void MatrizDispersa::generarReporte(int idCapa) {
    std::string nombreArchivo = "reporte_matriz_capa_" + std::to_string(idCapa) + ".dot";
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo de la matriz." << std::endl;
        return;
    }

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
        std::cout << "[OK] Reporte de matriz generado (vacía): " << nombreArchivo << std::endl;
        return;
    }

    // Nodo RAIZ
    archivo << "    N_R [label=\"RAIZ\", group=0, style=filled, fillcolor=lightgray];\n";

    // Cabeceras de columna en el mismo rank que la raíz
    NodoMatriz* col = raiz->derecha;
    archivo << "    { rank=same; N_R; ";
    while (col != raiz) {
        archivo << "N_C" << col->columna << "; ";
        col = col->derecha;
    }
    archivo << "}\n";

    col = raiz->derecha;
    while (col != raiz) {
        archivo << "    N_C" << col->columna
                << " [label=\"C" << col->columna
                << "\", group=" << col->columna
                << ", style=filled, fillcolor=lightblue];\n";
        col = col->derecha;
    }

    // Enlace horizontal entre cabeceras de columna
    archivo << "    N_R -> N_C" << raiz->derecha->columna << " [dir=both];\n";
    col = raiz->derecha;
    while (col->derecha != raiz) {
        archivo << "    N_C" << col->columna << " -> N_C" << col->derecha->columna << " [dir=both];\n";
        col = col->derecha;
    }

    // Recorrido por filas
    NodoMatriz* fila = raiz->abajo;
    while (fila != raiz) {
        archivo << "    N_F" << fila->fila
                << " [label=\"F" << fila->fila
                << "\", group=0, style=filled, fillcolor=lightpink];\n";

        // rank=same para cabecera de fila y sus píxeles
        archivo << "    { rank=same; N_F" << fila->fila << "; ";
        NodoMatriz* pxl = fila->derecha;
        while (pxl != fila) {
            archivo << "N_F" << pxl->fila << "_C" << pxl->columna << "; ";
            pxl = pxl->derecha;
        }
        archivo << "}\n";

        // Declarar píxeles con su color
        pxl = fila->derecha;
        while (pxl != fila) {
            std::string fillColor = pxl->colorHex;
            if (fillColor[0] != '#') fillColor = "#" + fillColor;
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna
                    << " [label=\"" << pxl->colorHex
                    << "\", group=" << pxl->columna
                    << ", style=filled, fillcolor=\"" << fillColor
                    << "\", fontcolor=white];\n";
            pxl = pxl->derecha;
        }

        // Enlace horizontal de esta fila
        archivo << "    N_F" << fila->fila << " -> N_F" << fila->fila
                << "_C" << fila->derecha->columna << " [dir=both];\n";
        pxl = fila->derecha;
        while (pxl->derecha != fila) {
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna
                    << " -> N_F" << pxl->derecha->fila << "_C" << pxl->derecha->columna
                    << " [dir=both];\n";
            pxl = pxl->derecha;
        }
        archivo << "    N_F" << pxl->fila << "_C" << pxl->columna
                << " -> N_F" << fila->fila << " [dir=both];\n";

        fila = fila->abajo;
    }

    // Enlace vertical entre cabeceras de fila
    archivo << "    N_R -> N_F" << raiz->abajo->fila << " [dir=both];\n";
    fila = raiz->abajo;
    while (fila->abajo != raiz) {
        archivo << "    N_F" << fila->fila << " -> N_F" << fila->abajo->fila << " [dir=both];\n";
        fila = fila->abajo;
    }

    // Enlace vertical entre nodos internos (por columna)
    col = raiz->derecha;
    while (col != raiz) {
        NodoMatriz* pxl = col->abajo;
        if (pxl != col) {
            archivo << "    N_C" << col->columna
                    << " -> N_F" << pxl->fila << "_C" << pxl->columna << " [dir=both];\n";
            while (pxl->abajo != col) {
                archivo << "    N_F" << pxl->fila << "_C" << pxl->columna
                        << " -> N_F" << pxl->abajo->fila << "_C" << pxl->abajo->columna
                        << " [dir=both];\n";
                pxl = pxl->abajo;
            }
            archivo << "    N_F" << pxl->fila << "_C" << pxl->columna
                    << " -> N_C" << col->columna << " [dir=both];\n";
        }
        col = col->derecha;
    }

    archivo << "}\n";
    archivo.flush();
    archivo.close();

    // Generar PNG automáticamente
    std::string pngFile = "reporte_matriz_capa_" + std::to_string(idCapa) + ".png";
    std::string cmd = "dot -Tpng \"" + nombreArchivo + "\" -o \"" + pngFile + "\" 2>&1";
    int ret = system(cmd.c_str());
    if (ret == 0)
        std::cout << "[OK] Reporte de matriz generado: " << pngFile << std::endl;
    else
        std::cout << "[OK] Archivo .dot generado: " << nombreArchivo
                  << " (instala Graphviz para generar PNG)" << std::endl;
}


ArbolCapas::ArbolCapas() {
    raiz = nullptr;
}

NodoArbolCapas* ArbolCapas::insertarRecursivo(NodoArbolCapas* nodo, int id) {
    if (nodo == nullptr) {
        NodoArbolCapas* nuevo = new NodoArbolCapas();
        nuevo->idCapa      = id;
        nuevo->raizMatriz  = nullptr;  // Se llena al cargar píxeles
        nuevo->izquierdo   = nullptr;
        nuevo->derecho     = nullptr;
        return nuevo;
    }
    if (id < nodo->idCapa)
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, id);
    else if (id > nodo->idCapa)
        nodo->derecho   = insertarRecursivo(nodo->derecho,   id);
    // Si id == nodo->idCapa, no se duplica
    return nodo;
}

void ArbolCapas::insertar(int id) {
    raiz = insertarRecursivo(raiz, id);
}

NodoArbolCapas* ArbolCapas::buscarRecursivo(NodoArbolCapas* nodo, int id) {
    if (nodo == nullptr || nodo->idCapa == id) return nodo;
    if (id < nodo->idCapa) return buscarRecursivo(nodo->izquierdo, id);
    return buscarRecursivo(nodo->derecho, id);
}

NodoArbolCapas* ArbolCapas::buscar(int id) {
    return buscarRecursivo(raiz, id);
}

NodoArbolCapas* ArbolCapas::getRaiz() {
    return raiz;
}

// Recorridos para generación de imagen por recorrido
void ArbolCapas::recorridoInorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado) {
    if (nodo == nullptr || contador >= limite) return;
    recorridoInorden(nodo->izquierdo, contador, limite, resultado);
    if (contador < limite) {
        resultado[contador++] = nodo;
    }
    recorridoInorden(nodo->derecho, contador, limite, resultado);
}

void ArbolCapas::recorridoPreorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado) {
    if (nodo == nullptr || contador >= limite) return;
    if (contador < limite) {
        resultado[contador++] = nodo;
    }
    recorridoPreorden(nodo->izquierdo, contador, limite, resultado);
    recorridoPreorden(nodo->derecho,   contador, limite, resultado);
}

void ArbolCapas::recorridoPostorden(NodoArbolCapas* nodo, int& contador, int limite, NodoArbolCapas** resultado) {
    if (nodo == nullptr || contador >= limite) return;
    recorridoPostorden(nodo->izquierdo, contador, limite, resultado);
    recorridoPostorden(nodo->derecho,   contador, limite, resultado);
    if (contador < limite) {
        resultado[contador++] = nodo;
    }
}

void ArbolCapas::generarImagenPorRecorrido(int numCapas, int tipoRecorrido) {
    if (raiz == nullptr) {
        std::cout << "[Error] El árbol de capas está vacío." << std::endl;
        return;
    }
    if (numCapas <= 0) {
        std::cout << "[Error] El número de capas debe ser mayor a 0." << std::endl;
        return;
    }

    NodoArbolCapas** resultado = new NodoArbolCapas*[numCapas];
    for (int i = 0; i < numCapas; i++) resultado[i] = nullptr;
    int contador = 0;

    if (tipoRecorrido == 1) {        // Inorden
        recorridoInorden(raiz, contador, numCapas, resultado);
    } else if (tipoRecorrido == 2) { // Preorden
        recorridoPreorden(raiz, contador, numCapas, resultado);
    } else {                          // Postorden
        recorridoPostorden(raiz, contador, numCapas, resultado);
    }

    std::string nombreRecorrido = (tipoRecorrido == 1) ? "inorden" :
                                  (tipoRecorrido == 2) ? "preorden" : "postorden";

    std::cout << "Capas seleccionadas en recorrido " << nombreRecorrido << ": ";
    for (int i = 0; i < contador; i++) {
        if (resultado[i]) std::cout << resultado[i]->idCapa << " ";
    }
    std::cout << std::endl;

    // Construir lista temporal de capas en el orden obtenido
    NodoListaCapas* cabeza = nullptr;
    NodoListaCapas* cola   = nullptr;
    for (int i = 0; i < contador; i++) {
        if (resultado[i] != nullptr) {
            NodoListaCapas* n = new NodoListaCapas();
            n->capaApuntada = resultado[i];
            n->siguiente    = nullptr;
            if (cabeza == nullptr) { cabeza = n; cola = n; }
            else { cola->siguiente = n; cola = n; }
        }
    }

    std::string salida = "imagen_recorrido_" + nombreRecorrido + "_" + std::to_string(numCapas);
    superponerCapasYGenerar(cabeza, salida);

    // Liberar lista temporal
    while (cabeza != nullptr) {
        NodoListaCapas* tmp = cabeza;
        cabeza = cabeza->siguiente;
        delete tmp;
    }
    delete[] resultado;
}

void ArbolCapas::generarDotRecursivo(NodoArbolCapas* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;
    archivo << "    " << nodo->idCapa
            << " [label=\"Capa " << nodo->idCapa << "\", shape=ellipse];\n";
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

    int ret = system("dot -Tpng reporte_capas.dot -o reporte_capas.png 2>&1");
    if (ret == 0)
        std::cout << "[OK] Reporte árbol de capas: reporte_capas.png" << std::endl;
    else
        std::cout << "[OK] Archivo generado: reporte_capas.dot" << std::endl;
}


ListaImagenes::ListaImagenes() {
    primero = nullptr;
}

void ListaImagenes::insertar(int id) {
    NodoImagen* nuevo = new NodoImagen();
    nuevo->idImagen  = id;
    nuevo->infoCapas = nullptr;
    nuevo->siguiente = nullptr;
    nuevo->anterior  = nullptr;

    if (primero == nullptr) {
        primero           = nuevo;
        primero->siguiente = primero;
        primero->anterior  = primero;
        return;
    }

    // Verificar duplicado antes de insertar
    NodoImagen* actual = primero;
    do {
        if (actual->idImagen == id) {
            delete nuevo;
            return; // ya existe
        }
        actual = actual->siguiente;
    } while (actual != primero);

    // Insertar ordenado
    if (id < primero->idImagen) {
        NodoImagen* ultimo  = primero->anterior;
        nuevo->siguiente    = primero;
        nuevo->anterior     = ultimo;
        primero->anterior   = nuevo;
        ultimo->siguiente   = nuevo;
        primero             = nuevo;
        return;
    }

    actual = primero;
    while (actual->siguiente != primero && actual->siguiente->idImagen < id) {
        actual = actual->siguiente;
    }

    nuevo->siguiente          = actual->siguiente;
    nuevo->anterior           = actual;
    actual->siguiente->anterior = nuevo;
    actual->siguiente         = nuevo;
}

bool ListaImagenes::eliminar(int id) {
    if (primero == nullptr) return false;

    NodoImagen* actual = primero;
    do {
        if (actual->idImagen == id) {
            if (actual->siguiente == actual) {
                // Único elemento
                primero = nullptr;
            } else {
                actual->anterior->siguiente = actual->siguiente;
                actual->siguiente->anterior = actual->anterior;
                if (actual == primero) primero = actual->siguiente;
            }
            // Liberar lista de capas del nodo
            NodoListaCapas* lc = actual->infoCapas;
            while (lc != nullptr) {
                NodoListaCapas* tmp = lc;
                lc = lc->siguiente;
                delete tmp;
            }
            delete actual;
            return true;
        }
        actual = actual->siguiente;
    } while (actual != primero);
    return false;
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
    nuevoNodoCapa->siguiente    = nullptr;

    if (img->infoCapas == nullptr) {
        img->infoCapas = nuevoNodoCapa;
    } else {
        NodoListaCapas* aux = img->infoCapas;
        while (aux->siguiente != nullptr) aux = aux->siguiente;
        aux->siguiente = nuevoNodoCapa;
    }
}

NodoImagen* ListaImagenes::getPrimero() {
    return primero;
}

void ListaImagenes::generarImagenPorId(int idImagen) {
    NodoImagen* img = buscar(idImagen);
    if (img == nullptr) {
        std::cout << "[Error] Imagen con ID " << idImagen << " no encontrada." << std::endl;
        return;
    }
    std::string salida = "imagen_" + std::to_string(idImagen);
    superponerCapasYGenerar(img->infoCapas, salida);
}

void ListaImagenes::generarReporteImagenYArbol(int idImagen, NodoArbolCapas* raizArbol) {
    NodoImagen* img = buscar(idImagen);
    if (img == nullptr) {
        std::cout << "[Error] Imagen con ID " << idImagen << " no encontrada." << std::endl;
        return;
    }

    std::string nombreDot = "reporte_imagen_arbol_" + std::to_string(idImagen) + ".dot";
    std::ofstream archivo(nombreDot);
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte." << std::endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "    rankdir=TB;\n";
    archivo << "    node [fontname=\"Arial\", shape=box];\n";
    archivo << "    label=\"IMAGEN " << idImagen << " + ARBOL DE CAPAS\";\n\n";

    // Nodo de la imagen
    archivo << "    \"Img_" << idImagen << "\" [label=\"Imagen " << idImagen
            << "\", style=filled, fillcolor=lightyellow, shape=rectangle];\n\n";

    // Lista de capas de la imagen con flechas al árbol
    NodoListaCapas* lc = img->infoCapas;
    int orden = 1;
    while (lc != nullptr) {
        int idC = lc->capaApuntada->idCapa;
        std::string nodoLC = "LC_" + std::to_string(idImagen) + "_" + std::to_string(orden);
        archivo << "    " << nodoLC << " [label=\"[" << orden << "] Capa " << idC
                << "\", style=filled, fillcolor=lightsalmon, shape=ellipse];\n";
        archivo << "    \"Img_" << idImagen << "\" -> " << nodoLC << ";\n";
        // Flecha hacia el nodo del árbol
        archivo << "    " << nodoLC << " -> " << idC
                << " [style=dashed, color=red, label=\"apunta\"];\n";
        lc = lc->siguiente;
        orden++;
    }

    // Árbol de capas completo (función lambda-like usando función auxiliar interna)
    // Recorremos el árbol recursivamente usando una función local embebida
    std::function<void(NodoArbolCapas*)> dotArbol = [&](NodoArbolCapas* nodo) {
        if (nodo == nullptr) return;
        archivo << "    " << nodo->idCapa
                << " [label=\"Capa " << nodo->idCapa << "\", shape=ellipse];\n";
        if (nodo->izquierdo) {
            archivo << "    " << nodo->idCapa << " -> " << nodo->izquierdo->idCapa << " [label=\"izq\"];\n";
            dotArbol(nodo->izquierdo);
        }
        if (nodo->derecho) {
            archivo << "    " << nodo->idCapa << " -> " << nodo->derecho->idCapa << " [label=\"der\"];\n";
            dotArbol(nodo->derecho);
        }
    };
    dotArbol(raizArbol);

    archivo << "}\n";
    archivo.flush();
    archivo.close();

    std::string pngFile = "reporte_imagen_arbol_" + std::to_string(idImagen) + ".png";
    std::string cmd = "dot -Tpng \"" + nombreDot + "\" -o \"" + pngFile + "\" 2>&1";
    int ret = system(cmd.c_str());
    if (ret == 0)
        std::cout << "[OK] Reporte imagen+árbol: " << pngFile << std::endl;
    else
        std::cout << "[OK] Archivo .dot generado: " << nombreDot << std::endl;
}

void ListaImagenes::generarReporte() {
    std::ofstream archivo("reporte_imagenes.dot");
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte de imágenes." << std::endl;
        return;
    }
    archivo << "digraph G {\n";
    archivo << "    rankdir=LR;\n";
    archivo << "    node [fontname=\"Arial\", shape=box];\n";
    archivo << "    label=\"LISTA CIRCULAR DOBLE DE IMAGENES\";\n";

    if (primero == nullptr) {
        archivo << "    \"Lista Vacia\" [shape=none];\n";
    } else {
        NodoImagen* actual = primero;
        do {
            archivo << "    \"Img_" << actual->idImagen
                    << "\" [label=\"Imagen " << actual->idImagen << "\"];\n";
            archivo << "    \"Img_" << actual->idImagen
                    << "\" -> \"Img_" << actual->siguiente->idImagen
                    << "\" [label=\"sig\"];\n";
            archivo << "    \"Img_" << actual->idImagen
                    << "\" -> \"Img_" << actual->anterior->idImagen
                    << "\" [label=\"ant\", style=dashed];\n";
            // Lista de capas de cada imagen
            NodoListaCapas* lc = actual->infoCapas;
            int pos = 1;
            while (lc != nullptr) {
                std::string nLC = "LC_" + std::to_string(actual->idImagen) + "_" + std::to_string(pos);
                archivo << "    " << nLC
                        << " [label=\"Capa " << lc->capaApuntada->idCapa
                        << "\", shape=ellipse, style=filled, fillcolor=lightblue];\n";
                if (pos == 1) {
                    archivo << "    \"Img_" << actual->idImagen << "\" -> " << nLC << " [label=\"capas\"];\n";
                } else {
                    std::string nAnterior = "LC_" + std::to_string(actual->idImagen) + "_" + std::to_string(pos-1);
                    archivo << "    " << nAnterior << " -> " << nLC << ";\n";
                }
                lc = lc->siguiente;
                pos++;
            }
            actual = actual->siguiente;
        } while (actual != primero);
    }
    archivo << "}\n";
    archivo.flush();
    archivo.close();

    int ret = system("dot -Tpng reporte_imagenes.dot -o reporte_imagenes.png 2>&1");
    if (ret == 0)
        std::cout << "[OK] Reporte lista de imágenes: reporte_imagenes.png" << std::endl;
    else
        std::cout << "[OK] Archivo generado: reporte_imagenes.dot" << std::endl;
}


ArbolUsuarios::ArbolUsuarios() {
    raiz = nullptr;
}

NodoUsuario* ArbolUsuarios::insertarRecursivo(NodoUsuario* nodo, std::string nombreUsuario) {
    if (nodo == nullptr) {
        NodoUsuario* nuevo    = new NodoUsuario();
        nuevo->nombre         = nombreUsuario;
        nuevo->imagenesPoseidas = nullptr;
        nuevo->izquierdo      = nullptr;
        nuevo->derecho        = nullptr;
        return nuevo;
    }
    if (nombreUsuario < nodo->nombre)
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, nombreUsuario);
    else if (nombreUsuario > nodo->nombre)
        nodo->derecho   = insertarRecursivo(nodo->derecho,   nombreUsuario);
    return nodo;
}

void ArbolUsuarios::insertar(std::string nombreUsuario) {
    raiz = insertarRecursivo(raiz, nombreUsuario);
}

NodoUsuario* ArbolUsuarios::minimoNodo(NodoUsuario* nodo) {
    while (nodo->izquierdo != nullptr) nodo = nodo->izquierdo;
    return nodo;
}

NodoUsuario* ArbolUsuarios::eliminarRecursivo(NodoUsuario* nodo, std::string nombreUsuario) {
    if (nodo == nullptr) return nullptr;
    if (nombreUsuario < nodo->nombre) {
        nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, nombreUsuario);
    } else if (nombreUsuario > nodo->nombre) {
        nodo->derecho = eliminarRecursivo(nodo->derecho, nombreUsuario);
    } else {
        // Encontrado
        if (nodo->izquierdo == nullptr) {
            NodoUsuario* tmp = nodo->derecho;
            // Liberar lista de imágenes
            NodoListaImagenesUsuario* li = nodo->imagenesPoseidas;
            while (li != nullptr) {
                NodoListaImagenesUsuario* t = li;
                li = li->siguiente;
                delete t;
            }
            delete nodo;
            return tmp;
        } else if (nodo->derecho == nullptr) {
            NodoUsuario* tmp = nodo->izquierdo;
            NodoListaImagenesUsuario* li = nodo->imagenesPoseidas;
            while (li != nullptr) {
                NodoListaImagenesUsuario* t = li;
                li = li->siguiente;
                delete t;
            }
            delete nodo;
            return tmp;
        }
        // Dos hijos: reemplazar con el mínimo del subárbol derecho
        NodoUsuario* sucesor = minimoNodo(nodo->derecho);
        nodo->nombre = sucesor->nombre;
        // Mover lista de imágenes del sucesor
        NodoListaImagenesUsuario* li = nodo->imagenesPoseidas;
        while (li != nullptr) {
            NodoListaImagenesUsuario* t = li;
            li = li->siguiente;
            delete t;
        }
        nodo->imagenesPoseidas = sucesor->imagenesPoseidas;
        sucesor->imagenesPoseidas = nullptr;
        nodo->derecho = eliminarRecursivo(nodo->derecho, sucesor->nombre);
    }
    return nodo;
}

bool ArbolUsuarios::eliminar(std::string nombreUsuario) {
    if (buscar(nombreUsuario) == nullptr) return false;
    raiz = eliminarRecursivo(raiz, nombreUsuario);
    return true;
}

NodoUsuario* ArbolUsuarios::buscar(std::string nombreUsuario) {
    NodoUsuario* actual = raiz;
    while (actual != nullptr) {
        if (nombreUsuario == actual->nombre) return actual;
        if (nombreUsuario < actual->nombre)  actual = actual->izquierdo;
        else                                  actual = actual->derecho;
    }
    return nullptr;
}

void ArbolUsuarios::agregarImagenAUsuario(std::string nombreUsuario, int idImagen) {
    NodoUsuario* user = buscar(nombreUsuario);
    if (user == nullptr) return;

    // Verificar duplicado
    NodoListaImagenesUsuario* aux = user->imagenesPoseidas;
    while (aux != nullptr) {
        if (aux->idImagen == idImagen) return;
        aux = aux->siguiente;
    }

    NodoListaImagenesUsuario* nuevoItem = new NodoListaImagenesUsuario();
    nuevoItem->idImagen  = idImagen;
    nuevoItem->siguiente = nullptr;

    if (user->imagenesPoseidas == nullptr) {
        user->imagenesPoseidas = nuevoItem;
    } else {
        aux = user->imagenesPoseidas;
        while (aux->siguiente != nullptr) aux = aux->siguiente;
        aux->siguiente = nuevoItem;
    }
}

void ArbolUsuarios::eliminarImagenDeUsuario(std::string nombreUsuario, int idImagen) {
    NodoUsuario* user = buscar(nombreUsuario);
    if (user == nullptr) return;

    NodoListaImagenesUsuario* actual   = user->imagenesPoseidas;
    NodoListaImagenesUsuario* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->idImagen == idImagen) {
            if (anterior == nullptr) user->imagenesPoseidas = actual->siguiente;
            else                     anterior->siguiente    = actual->siguiente;
            delete actual;
            return;
        }
        anterior = actual;
        actual   = actual->siguiente;
    }
}

NodoUsuario* ArbolUsuarios::getRaiz() {
    return raiz;
}

void ArbolUsuarios::listarNombresRecursivo(NodoUsuario* nodo) {
    if (nodo == nullptr) return;
    listarNombresRecursivo(nodo->izquierdo);
    std::cout << "  - " << nodo->nombre << std::endl;
    listarNombresRecursivo(nodo->derecho);
}

void ArbolUsuarios::listarUsuarios() {
    if (raiz == nullptr) {
        std::cout << "  (No hay usuarios registrados)" << std::endl;
        return;
    }
    listarNombresRecursivo(raiz);
}

void ArbolUsuarios::generarDotRecursivo(NodoUsuario* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;
    archivo << "    \"" << nodo->nombre
            << "\" [label=\"" << nodo->nombre << "\", shape=box];\n";

    // Nodos de imágenes del usuario
    NodoListaImagenesUsuario* li = nodo->imagenesPoseidas;
    int pos = 1;
    while (li != nullptr) {
        std::string nImg = "\"Img_" + nodo->nombre + "_" + std::to_string(pos) + "\"";
        archivo << "    " << nImg
                << " [label=\"img " << li->idImagen
                << "\", shape=ellipse, style=filled, fillcolor=lightsalmon];\n";
        if (pos == 1)
            archivo << "    \"" << nodo->nombre << "\" -> " << nImg << ";\n";
        else {
            std::string nAnterior = "\"Img_" + nodo->nombre + "_" + std::to_string(pos-1) + "\"";
            archivo << "    " << nAnterior << " -> " << nImg << ";\n";
        }
        li = li->siguiente;
        pos++;
    }

    if (nodo->izquierdo) {
        archivo << "    \"" << nodo->nombre << "\" -> \"" << nodo->izquierdo->nombre << "\" [label=\"izq\"];\n";
        generarDotRecursivo(nodo->izquierdo, archivo);
    }
    if (nodo->derecho) {
        archivo << "    \"" << nodo->nombre << "\" -> \"" << nodo->derecho->nombre << "\" [label=\"der\"];\n";
        generarDotRecursivo(nodo->derecho, archivo);
    }
}

void ArbolUsuarios::generarReporte() {
    std::ofstream archivo("reporte_usuarios.dot");
    if (!archivo.is_open()) {
        std::cout << "[Error] No se pudo crear el archivo del reporte de usuarios." << std::endl;
        return;
    }
    archivo << "digraph G {\n";
    archivo << "    node [fontname=\"Arial\", shape=box];\n";
    archivo << "    label=\"REPORTE ARBOL DE USUARIOS\";\n";

    if (raiz == nullptr) {
        archivo << "    \"Arbol de Usuarios Vacio\" [shape=none];\n";
    } else {
        generarDotRecursivo(raiz, archivo);
    }
    archivo << "}\n";
    archivo.flush();
    archivo.close();

    int ret = system("dot -Tpng reporte_usuarios.dot -o reporte_usuarios.png 2>&1");
    if (ret == 0)
        std::cout << "[OK] Reporte árbol de usuarios: reporte_usuarios.png" << std::endl;
    else
        std::cout << "[OK] Archivo generado: reporte_usuarios.dot" << std::endl;
}