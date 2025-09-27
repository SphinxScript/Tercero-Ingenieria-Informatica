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
  std::vector<unsigned> camino, inspeccionado;
  --nodo_inicio;
  bool nodo_final_encontrado = false;
  Dfs(nodo_inicio, visitados, camino, inspeccionado, nodo_final, nodo_final_encontrado);
  std::cout << "Camino: " << std::endl;
  for (auto& elemento : camino) {
    std::cout << elemento << " ";
  }
  std::cout << std::endl;
  for (auto& elemento : inspeccionado) {
    std::cout << elemento << " ";
  }
  std::cout << std::endl;
}

void Grafo::Dfs(const unsigned& nodo_actual, std::vector<bool>& visitado, std::vector<unsigned>& camino,
                std::vector<unsigned>& inspeccionado, const unsigned& nodo_final, bool& nodo_final_encontrado) const {
  visitado[nodo_actual] = true;   // Aquí marcamos el nodo actual como visitado
  camino.push_back(nodo_actual + 1);

  if (nodo_actual == (nodo_final -1)) { // hemos restado 1 al nodo_actual, asi que hacemos lo mismo con nodo final
    inspeccionado.push_back(nodo_final);
    nodo_final_encontrado = true;
    return;
  }
  for (int i{0}; i < lista_adyacencia_[nodo_actual].size() && !nodo_final_encontrado; ++i) {    // bucle para recorrer los vecinos del nodo
    if (!visitado[lista_adyacencia_[nodo_actual][i].nodo - 1] && !nodo_final_encontrado) {          // si no está visitado el vecino, se llama a Dfs con el vecino
      inspeccionado.push_back(nodo_actual + 1);
      std::cout << lista_adyacencia_[nodo_actual][i].nodo << " ";
      Dfs(lista_adyacencia_[nodo_actual][i].nodo - 1, visitado,camino, inspeccionado, nodo_final, nodo_final_encontrado);
      camino.pop_back();
      std::cout << std::endl;
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