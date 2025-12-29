/**
 * @file auxiliares.h
 * @brief Fichero de cabecera de las funciones auxiliares
 * @author Ricardo David Rodríguez Pane
 * @date 25-12-2025
 * 
 */

#include <iostream>
#include "auxiliares.h"
#include "grafo.h"

/**
 * @brief Muestra la ayuda del programa
 * @return void
 */
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

/**
 * @brief Comprueba los argumentos pasados al programa
 * @param argc número de argumentos
 * @param argv vector de argumentos
 * @return true si se ha pedido ayuda, false en caso contrario
 */
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

/**
 * @brief Maneja la entrada del usuario para los nodos inicial y final
 * @param rango número máximo de nodos en el grafo
 * @return par de enteros con el nodo inicial y el nodo final
 */
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

/**
 * @brief Imprime el camino desde el nodo inicial al nodo final del recorrido DFS
 * @param padres vector de nodos que almacena el orden de visita
 * @param nodo_final nodo final del camino
 * @return void
 */
void PrintDfs(const std::vector<Nodo>& padres, int nodo_final, std::ostream& os) {
  std::vector<int> hijos;
  int v = nodo_final;
  while (v != -1) {
    hijos.push_back(v + 1);
    v = padres[v].padre;
  }
  for (int i{static_cast<int>(hijos.size() - 1)}; i >= 0; --i) {
    if (i == 0) {
      os << hijos[i];
      break;
    }
    os << hijos[i] << " -> ";
  }
  os << "\nCoste: " << padres[nodo_final].coste_acumulado;
  os << std::endl;
}

void PrintSummary(std::ostream& os, const Grafo& grafo, std::pair<int, int> inicio_fin) {
  os << "Número de nodos del grafo: " << grafo.GetVertices() << std::endl;
  os << "Número de aristas del grafo: " << grafo.GetAristas() << std::endl;
  os << "Vértice origen: " << inicio_fin.first << std::endl;
  os << "Vértice destino: " << inicio_fin.second << std::endl;
}