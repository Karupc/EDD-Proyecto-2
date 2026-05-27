#ifndef SISTEMA_H
#define SISTEMA_H

#include "estructuras.h"
#include <string>

class CargaMasiva {
public:
    void cargarCapas(std::string ruta, ArbolCapas& arbol);
    void cargarImagenes(std::string ruta, ListaImagenes& lista, ArbolCapas& arbol);
    void cargarUsuarios(std::string ruta, ArbolUsuarios& arbolUsr);
};

#endif