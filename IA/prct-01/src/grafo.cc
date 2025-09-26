#include "grafo.h"
#include <queue>

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
  int contador_aristas{0};
  for (int i{0}; i < nodos_ - 1; ++i) {
    for (int j{i + 1}; j < nodos_; ++j) {
      fichero >> costo;
      if (costo != -1) {
        ++contador_aristas;
        lista_adyacencia_[i].push_back(ElementoAdyacencia(j + 1, costo));
        lista_adyacencia_[j].push_back(ElementoAdyacencia(i + 1, costo));
      }
    }
  }
  arcos_ = contador_aristas;
  fichero.close();
}

// Método que imprime un resumen del grafo
void Grafo::ImprimeResumen() const {
  std::cout << "Número de nodos del grafo: " << nodos_ << std::endl;
  std::cout << "Número de aristas del grafo: " << arcos_ << std::endl;
}

bool Grafo::CompruebaDestino(unsigned& nodo_destino, unsigned& nodo_actual) const {
  return (nodo_actual == nodo_destino) ? true : false;
}


// Método de recorrido en profundidad
void Grafo::RecorridoProfundidad(unsigned& nodo_inicio, unsigned& nodo_final) const {
  Grafo grafo = *this;
  grafo.ImprimeResumen();
  std::cout << "Vértice origen: " << nodo_inicio << std::endl;
  std::cout << "Vértice destino: " << nodo_final << std::endl;
  std::vector<bool> visitados;
  visitados.resize(nodos_, false);
  std::vector<unsigned> prenum, postnum;
  prenum.resize(nodos_);
  postnum.resize(nodos_);
  unsigned prenum_ind{0}, postnum_ind{0};
  std::vector<nodo_lista> lista_adyac_dfs;
  lista_adyac_dfs = lista_adyacencia_;
  --nodo_inicio; //restamos ya que está indexado con una unidad menos.
  Dfs(nodo_inicio, lista_adyac_dfs, visitados, prenum, prenum_ind, postnum, postnum_ind);
}

void Grafo::Dfs(unsigned& nodo_actual, std::vector<nodo_lista> lista, std::vector<bool>& visitado, std::vector<unsigned>& vector_prenum,
                unsigned& prenum_ind, std::vector<unsigned>& vector_postnum, unsigned& postnum_ind) const {
  std::queue<unsigned> pila_nodos;
  for (int i{nodo_actual}; i < lista[i].size(); ++i) {
    if (!visitado[i]) {
      visitado[i] = true;
      pila_nodos.emplace(lista[i][j].nodo);   //emplazamos en la cola los sucesores del nodo actual
    }
    for (int j{0}; j < lista[i].size(); ++j) {

    }
  }
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