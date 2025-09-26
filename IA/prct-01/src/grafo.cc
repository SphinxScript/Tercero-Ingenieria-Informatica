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
  fichero >> nodos_;
  lista_adyacencia_.resize(nodos_);

  for (int i{0}; i < nodos_ - 1; ++i) {
    for (int j{i + 1}; j < nodos_; ++j) {
      fichero >> costo;
      if (costo != -1) {
      lista_adyacencia_[i].push_back(ElementoAdyacencia(j + 1, costo));
      lista_adyacencia_[j].push_back(ElementoAdyacencia(i + 1, costo));
      }
    }
  }
  fichero.close();
  
}

std::ostream& operator<<(std::ostream& os, const Grafo& grafo) {
  std::cout << "Lista de Adyacencia del grafo:" << std::endl;
  std::vector<nodo_lista> lista{grafo.GetLista()};
  for (int i{0}; i < lista.size(); ++i) {

    std::cout << "Adyacentes de " << i + 1;
    if (lista[i].size() == 0) {
      std::cout << ": []";
    }
    else {
      std::cout << ": [";
    }
    for (int j{0}; j < lista[i].size() && lista[i].size() != 0; ++j) {

      if (j == lista[i].size() - 1) {
        std::cout << lista[i][j].nodo << "]";
        break;
      }
      std::cout << lista[i][j].nodo << ", ";
    }
    std::cout << std::endl;
  }
  return os;
}