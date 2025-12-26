/**
 * @file grafo.h
 * @brief fichero de cabecera para almacenar la clase Grafo
 * @author Ricardo David Rodríguez Pane
 * @date 26-12-2025
 * 
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <fstream>

// struct para representar los nodos (en la búsqueda)
// almacena nodo padre, nodo hijo y coste al nodo hijo
struct Nodo{
  Nodo* nodo_padre;
  int nodo_hijo;
  int coste_hijo;
  int coste_acumulado;
};

class Grafo {
 public:
  Grafo();
  Grafo(std::ifstream&);
  bool RecorridoProfundidad(int, int);
  auto GetMatrix() { return matr_adyac_; }

 private:
  // matriz para almacenar la adyacencia
  // indexado desde -1 (posición n en la matriz corresponde a la arista n + 1 del grafo, pero se trabaja siempre dentro con n)
  std::vector<std::vector<double>> matr_adyac_;
  void BuildGraph(std::ifstream&);
  int n_aristas_;
  bool dfs(int, int);

};

#endif