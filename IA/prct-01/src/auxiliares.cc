/**
 * @file auxiliares.h
 * @brief Fichero de cabecera de las funciones auxiliares
 * @author Ricardo David Rodríguez Pane
 * @date 25-12-2025
 * 
 */

#include <iostream>
#include "auxiliares.h"

void ShowHelp() {
  std::cout
    << "Modo de uso:\n"
    << "   ./busqueda.out <fichero_entrada> <fichero_salida>\n"
    << "   ./busqueda.out --help\n"
    << "   ./busqueda.out -h\n\n"
    << "Opciones:\n"
    << "  --help   Muestra esta ayuda\n"
    << "  -h       Muestra esta ayuda\n";
}
