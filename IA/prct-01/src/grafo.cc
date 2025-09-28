#include "grafo.h"
#include <stack>
#include <algorithm>
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
        lista_adyacencia_[i].push_back(ElementoAdyacencia(j, costo));
        lista_adyacencia_[j].push_back(ElementoAdyacencia(i, costo));
      }
    }
  }
  arcos_ = contador_aristas;
  fichero.close();
}

// Método que imprime un resumen del grafo
void Grafo::ImprimeResumen() const {
  std::cout << "--------------------------" << std::endl;
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
//  std::vector<bool> visitados;
//  visitados.resize(nodos_, false);
  std::vector<unsigned> camino;
//  --nodo_inicio;
//  bool nodo_final_encontrado = false;
//  std::vector<unsigned> generado;
  std::vector<unsigned> padres;
  --nodo_inicio;
  --nodo_final;
  Dfs(nodo_inicio, nodo_final, padres);
  camino.clear();
  unsigned nodo = nodo_final;
  while (true) {
    camino.push_back(nodo);
    if (nodo == nodo_inicio) break;
    nodo = padres[nodo];
  }
  std::reverse(camino.begin(), camino.end());
  std::cout << "Camino: ";
  for (auto n : camino) {
    std::cout << n + 1 << " ";
  }
  std::cout << std::endl;
  double coste = CalculaCoste(camino);
  std::cout << "Coste: " << coste << std::endl;
  std::cout << "--------------------------" << std::endl;
}

void Grafo::Dfs(const unsigned& nodo_inicial, const unsigned& nodo_final, std::vector<unsigned>& padres) const {
  std::stack<unsigned> pila_nodos;
  pila_nodos.push(nodo_inicial);
  std::vector<bool> visitados;
  visitados.resize(nodos_, false);
  bool encontrado = false;
  int contador{0};
  padres.resize(nodos_);
  std::vector<unsigned> generados;
  std::vector<unsigned> inspeccionados;
  unsigned nodo_actual;
  generados.push_back(nodo_inicial);
  while (!pila_nodos.empty() && !encontrado) {
    if (nodo_actual == nodo_final) {
      encontrado = true;
      continue;
    }
    std::cout << "--------------------------" << std::endl;
    std::cout << "Iteración: " << ++contador << std::endl;
    std::cout << "Nodos generados: ";
    PrintVector(generados);
    std::cout << "Nodos inspeccionados: ";
    PrintVector(inspeccionados);
    nodo_actual = pila_nodos.top();
    pila_nodos.pop();
    visitados[nodo_actual] = true;
    inspeccionados.push_back(nodo_actual);
//    std::cout << "Debug: nodos a explorar: ";
//    int n = lista_adyacencia_[nodo_actual].size();
    for (int i{lista_adyacencia_[nodo_actual].size() - 1}; i >= 0; --i) {   // Bucle for para insertar en la pila en orden inverso,y en el vector en orden creciente
      unsigned siguiente = lista_adyacencia_[nodo_actual][i].nodo;
      if (!visitados[siguiente]) {
        pila_nodos.push(lista_adyacencia_[nodo_actual][i].nodo);
        padres[siguiente] = nodo_actual;    // el padre del siguiente es el que estoy ahora
        //generados.push_back(siguiente);
        //std::cout << " " << siguiente + 1;
        //padres[siguiente] = nodo_actual;
      }
    }
    for (int i{0}; i < lista_adyacencia_[nodo_actual].size(); ++i) {   // Bucle for para insertar en la pila en orden inverso,y en el vector en orden creciente
      unsigned siguiente = lista_adyacencia_[nodo_actual][i].nodo;
      if (!visitados[siguiente]) {
        generados.push_back(lista_adyacencia_[nodo_actual][i].nodo);
//        generados.push_back(siguiente);
//        std::cout << " " << siguiente + 1;
//        padres[siguiente] = nodo_actual;
      }
    }
  }
  std::cout << "--------------------------" << std::endl;
}

void Grafo::PrintVector (const std::vector<unsigned>& vector) const {
  if (vector.size() == 0) {
    std::cout << "-";
  }
  else {
    for (const unsigned& elemento : vector) {
      std::cout << elemento + 1 << " ";
    }
  }
  
  std::cout << std::endl;
}

double Grafo::CalculaCoste(const std::vector<unsigned>& camino) const {
  double coste_total = 0.0;
  for (int i{0}; i < camino.size(); ++i) {
    unsigned nodo_actual = camino[i];
    unsigned nodo_sig = camino[i + 1];
    for (const auto& elemento : lista_adyacencia_[nodo_actual]) {
      if (elemento.nodo == nodo_sig) {
        coste_total += elemento.coste;
        break;
      }
    }
  }
  return coste_total;
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
        std::cout << lista[i][j].nodo + 1 << "]";
        break;
      }
      std::cout << lista[i][j].nodo + 1 << ", ";
    }
    std::cout << std::endl;
  }
  return os;
}