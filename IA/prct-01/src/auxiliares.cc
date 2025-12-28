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

bool CheckArgs(const int argc, char* const argv[]) {
  bool help = false;
  for (int i{1}; i < argc; ++i) {
    if (std::string(argv[i]) == "--h" || std::string(argv[i]) == "--help") {
      help = true;
      return help;
    }
    else continue;
  }
  return help;
}

std::pair<int, int> HandleNode(int rango) {
  int nodo_inicio;
  while (nodo_inicio > rango || nodo_inicio < 1) {
    std::cout << "Introduzca un nodo inicial del grafo.\n"
              << "Rango entre: 1 y " << rango << " : ";
    std::cin >> nodo_inicio;
    if (nodo_inicio > rango || nodo_inicio < 1) {
      std::cout << "Nodo fuera de rango. Repita." << std::endl;
    }
  }
  std::cout << std::endl;
  int nodo_final;
  while (nodo_final > rango || nodo_final < 1) {
    std::cout << "Introduzca un nodo final del grafo.\n"
              << "Rango entre: 1 y " << rango << " : ";
    std::cin >> nodo_final;
    if (nodo_final > rango || nodo_final < 1) {
      std::cout << "Nodo fuera de rango. Repita." << std::endl;
    }
  }
  return std::pair<int, int>(nodo_inicio, nodo_final);
}

void PrintDfs(const std::vector<Nodo>& padres, int nodo_final) {
  std::vector<int> hijos;
  int v = nodo_final;
  --v;
  hijos.push_back(nodo_final + 1);
  while (v != -1) {
    hijos.push_back(v + 1);
    v = padres[v].padre;
  }
  for (int i{hijos.size() - 1}; i >= 0; --i) {
    if (i == 0) {
      std::cout << hijos[i];
      break;
    }
    std::cout << hijos[i] << " -> ";
  }
  std::cout << "\nCoste: " << padres[nodo_final].coste_acumulado;
  std::cout << std::endl;
}