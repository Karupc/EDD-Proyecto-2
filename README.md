# Generador de Imágenes por Capas (EDD Layered Image Processor)

Este proyecto consiste en un sistema de procesamiento y renderizado de imágenes compuestas por múltiples capas de píxeles, desarrollado en **C++** como parte del curso de Estructuras de Datos. La aplicación gestiona la memoria dinámica mediante estructuras de datos complejas hechas desde cero (sin dependencias de la biblioteca estándar para el almacenamiento) y exporta representaciones visuales automáticas utilizando **Graphviz**.

---

## Requisitos del Sistema

* **Compilador:** Compatible con C++11 o superior (ej. `g++`).
* **Graphviz:** Necesario para compilar y generar las vistas lógicas y los mapas de píxeles en formato `.png` a partir de los archivos `.dot`.
* **Codificación:** Configurado con soporte UTF-8 nativo para asegurar la correcta legibilidad de menús en consolas Windows, Mac y Linux.

---

## Compilación y Ejecución

Para compilar el proyecto de forma manual desde tu terminal, clona este repositorio y ejecuta los siguientes comandos:

```bash
# Compilación del ejecutable principal
g++ -std=c++11 main.cpp estructuras.cpp sistema.cpp -o SistemaCapas

# Ejecución de la aplicación
./SistemaCapas
