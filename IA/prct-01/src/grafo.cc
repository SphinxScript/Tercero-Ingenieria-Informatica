/**
 * @file grafo.cc
 * @brief fichero para implementar la clase Grafo
 * @author Ricardo David Rodríguez Pane
 * @date 26-12-2025
 */

#include <fstream>
#include <iostream>

#include "grafo.h"


Grafo::Grafo(std::ifstream& flujo_entrada) {
  BuildGraph(flujo_entrada);
}


void Grafo::BuildGraph(std::ifstream& flujo_entrada) {
  flujo_entrada >> n_aristas_;
  matr_adyac_.resize(n_aristas_, std::vector<double>(n_aristas_));
  for (int i{0}; i < n_aristas_; ++i) {
    for (int j{i + 1}; j < n_aristas_; ++j) {
      // la matriz en la posición i,j conecta ambos nodos con el peso en el fichero de entrada
      // como es simetrica (grafo no dirigido) también en la posición j, i
      double coste;
      flujo_entrada >> coste;
      matr_adyac_[i][j] = coste;
      matr_adyac_[j][i] = coste;
    }
  }
}