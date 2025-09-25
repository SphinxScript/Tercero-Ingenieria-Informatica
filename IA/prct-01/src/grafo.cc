#include "grafo.h"


// Definimos el constructor

Grafo::Grafo(const std::string& nombre_fichero, int& error_apertura) {
  Build(nombre_fichero, error_apertura);
}

void Grafo::Build(const std::string& fichero_string, int& error) {
  std::ifstream fichero{fichero_string};
  if (!fichero.is_open()) {
    std::cout << "Error al abrir el fichero" << std::endl;
    error = 1;
    return;
  }
  unsigned antecesor, predecesor;
  double costo;
  fichero >> nodos;
  ListaAdyacencia_.resize(nodos);

  for (int i{0}; i < nodos - 1; ++i) {
    for (int j{i + 1}; j < nodos; ++j) {
      fichero >> costo;
      ListaAdyacencia_[i].push_back(ElementoAdyacencia(j, costo));
      ListaAdyacencia_[j].push_back(ElementoAdyacencia(i, costo));
    }
  }
}