/**
 * @file grafo.cc
 * @brief fichero para implementar la clase Grafo
 * @author Ricardo David Rodríguez Pane
 * @date 26-12-2025
 */

#include <fstream>
#include <iostream>
#include <stack>
#include <queue>
#include <random>
#include <vector>

#include "grafo.h"

/**
 * @brief Constructor por defecto de la clase Grafo
 * @param flujo_entrada flujo de entrada desde el que se construye el grafo
 * @note llama a la función BuildGraph para construir el grafo
 * @return -
 */
Grafo::Grafo(std::ifstream& flujo_entrada) {
  BuildGraph(flujo_entrada);
}

/**
 * @brief Construye el grafo a partir del flujo de entrada
 * @param flujo_entrada flujo de entrada desde el que se construye la matriz de adyacencia
 * @return void
 */
void Grafo::BuildGraph(std::ifstream& flujo_entrada) {
  flujo_entrada >> n_vertices_;
  matr_adyac_.resize(n_vertices_, std::vector<double>(n_vertices_));
  n_aristas_ = 0;
  for (int i{0}; i < n_vertices_; ++i) {
    for (int j{i + 1}; j < n_vertices_; ++j) {
      // la matriz en la posición i,j conecta ambos nodos con el peso en el fichero de entrada
      // como es simetrica (grafo no dirigido) también en la posición j, i
      double coste;
      flujo_entrada >> coste;
      matr_adyac_[i][j] = coste;
      matr_adyac_[j][i] = coste;
      ++n_aristas_;
    }
  }
}

/**
 * @brief Realiza el recorrido en profundidad desde el nodo inicio al nodo fin
 * @param inicio_fin par de enteros que representan el nodo de inicio y el nodo final
 * @param padres vector de nodos que almacena el orden de visita
 * @note llama a la función Dfs para realizar el recorrido
 * @return true si se encuentra el nodo fin, false en caso contrario
 */
bool Grafo::RecorridoProfundidad(std::pair<int, int> inicio_fin, std::vector<Nodo>& padres, std::ostream& os) {
  int inicio = inicio_fin.first;
  int fin = inicio_fin.second;
  --inicio;
  --fin;
  return RecorridoAleatorio(inicio, fin, padres, os);
}

/**
 * @brief Realiza el recorrido en profundidad desde el nodo inicio al nodo fin
 * imprime durante las iteraciones los nodos generados e inspeccionados
 * @param nodo_inicio nodo desde el que se inicia el recorrido
 * @param arista_fin nodo que se quiere alcanzar
 * @param padres vector de nodos que almacena el orden de visita
 * @return true si se encuentra el nodo fin, false en caso contrario
 */
bool Grafo::Dfs(int nodo_inicio, int arista_fin, std::vector<Nodo>& padres, std::ostream& os) {
  os << "-------------------------------------------------------------------------------" << std::endl;
  os << "Recorrido en Profundidad" << std::endl;
  bool encontrado = false;
  std::vector<bool> visitado;
  visitado.resize(n_vertices_, false);
  std::stack<int> pila_nodos;
  visitado[nodo_inicio] = true;
  pila_nodos.push(nodo_inicio);
  padres.resize(n_vertices_);
  for (int i{0}; i < n_vertices_; ++i) {
    padres[i].id = i;
    padres[i].padre = -1;
    padres[i].coste = 0;
    padres[i].coste_acumulado = 0;
  }
  padres[nodo_inicio].coste = 0;
  padres[nodo_inicio].coste_acumulado = 0;
  padres[nodo_inicio].padre = -1;
  int contador{1};
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  generados.push_back(nodo_inicio);
  while (!pila_nodos.empty() && !encontrado) {
    int nodo_actual = pila_nodos.top();
    pila_nodos.pop();
    if (nodo_actual == arista_fin) {
      encontrado = true;
      break;
    }
    os << std::endl << "-------------------------------------------------------------------------------" << std::endl;
    os << "Iteración " << contador << std::endl;
    os << "Generados: ";
    for (auto& generado : generados) {
      os << generado + 1 << ", ";
    }
    os << std::endl;
    for (int i{0}; i < n_vertices_; ++i) {
      if (!visitado[i] && matr_adyac_[nodo_actual][i] != -1) {
        generados.push_back(i);
        visitado[i] = true;
        padres[i].padre = nodo_actual;
        padres[i].coste = matr_adyac_[nodo_actual][i];
        padres[i].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[i].coste;
        pila_nodos.push(i);
      }
    }
    os << "Inspeccionados: ";
    for (auto& inspeccionado : inspeccionados) {
      os << inspeccionado + 1 << ", ";
    }
    inspeccionados.push_back(nodo_actual);
    os << std::endl;
    ++contador;
  }
  os << "-------------------------------------------------------------------------------" << std::endl;
  return encontrado;
}

/**
 * @brief Realiza el recorrido en amplitud desde el nodo inicio al nodo fin
 * @param inicio_fin par de enteros que representan el nodo de inicio y el nodo final
 * @param padres vector de nodos que almacena el orden de visita
 * @note llama a la función Bfs para realizar el recorrido
 * @return true si se encuentra el nodo fin, false en caso contrario
 */
bool Grafo::RecorridoAmplitud(std::pair<int, int> inicio_fin, std::vector<Nodo>& padres, std::ostream& os) {
  int nodo_inicio = inicio_fin.first;
  int nodo_fin = inicio_fin.second;
  --nodo_inicio;
  --nodo_fin;
  return MejorPeor(nodo_inicio, nodo_fin, padres, os);
}

/**
 * @brief Realiza el recorrido en amplitud desde el nodo inicio al nodo fin
 * imprime durante las iteraciones los nodos generados e inspeccionados
 * @param vertice_inicio nodo desde el que se inicia el recorrido
 * @param vertice_final nodo que se quiere alcanzar
 * @param padres vector de nodos que almacena el orden de visita
 * @return true si se encuentra el nodo fin, false en caso contrario
 */
bool Grafo::Bfs(int vertice_inicio, int vertice_final, std::vector<Nodo>& padres, std::ostream& os) {
  os << "-------------------------------------------------------------------------------" << std::endl;
  os << "Recorrido en Amplitud" << std::endl;
  std::vector<bool> visitado;
  visitado.resize(n_vertices_, false);
  std::queue<int> cola_nodos;
  visitado[vertice_inicio] = true;
  cola_nodos.push(vertice_inicio);

  bool encontrado = false;
  padres.resize(n_vertices_);
  for (int i{0}; i < n_vertices_; ++i)  {
    padres[i].id = i;
    padres[i].padre = -1;
    padres[i].coste = 0;
    padres[i].coste_acumulado = 0;
  }
  padres[vertice_inicio].coste = 0;
  padres[vertice_inicio].coste_acumulado = 0;
  padres[vertice_inicio].padre = -1;
  int contador{1};
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  generados.push_back(vertice_inicio);
  while(!cola_nodos.empty() && !encontrado) {
    int nodo_actual = cola_nodos.front();
    cola_nodos.pop();
    if (nodo_actual == vertice_final) {
      encontrado = true;
      break;
    }
    os << std::endl << "-------------------------------------------------------------------------------" << std::endl;
    os << "Iteración " << contador << std::endl;
    os << "Nodos generados: ";
    for (const auto& nodo : generados) {
      os << nodo + 1 << ", ";
    }
    os << std::endl;
    for (int i{0}; i < n_vertices_; ++i) {
      if (!visitado[i] && matr_adyac_[nodo_actual][i] != -1) {
        visitado[i] = true;
        cola_nodos.push(i);
        padres[i].padre = nodo_actual;
        padres[i].coste = matr_adyac_[nodo_actual][i];
        padres[i].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[i].coste;
        generados.push_back(i);
      }
    }
    os << "Nodos inspeccionados: ";
    for (const auto& nodo : inspeccionados) {
      os << nodo + 1 << ", ";
    }
    os << std::endl;
    inspeccionados.push_back(nodo_actual);
    ++contador;
  }
  return encontrado;
}


// DFS que extrae de los nodos aleatoriamente para el recorrido en profundidad
bool Grafo::RecorridoAleatorio(int nodo_inicio, int arista_fin, std::vector<Nodo>& padres, std::ostream& os) {
  os << "-------------------------------------------------------------------------------" << std::endl;
  os << "Recorrido en Profundidad aleatorio: " << std::endl;

  std::mt19937 generador(std::random_device{}());
  bool encontrado = false;
  std::vector<bool> visitado;
  visitado.resize(n_vertices_, false);
  std::vector<int> vector_nodos;
  visitado[nodo_inicio] = true;
  vector_nodos.push_back(nodo_inicio);
  padres.resize(n_vertices_);
  for (int i{0}; i < n_vertices_; ++i) {
    padres[i].id = i;
    padres[i].padre = -1;
    padres[i].coste = 0;
    padres[i].coste_acumulado = 0;
  }
  padres[nodo_inicio].coste = 0;
  padres[nodo_inicio].coste_acumulado = 0;
  padres[nodo_inicio].padre = -1;
  int contador{1};
  std::vector<int> generados;
  std::vector<int> inspeccionados;
  generados.push_back(nodo_inicio);
  while (!vector_nodos.empty() && !encontrado) {
    // escogemos un índice aleatorio del vector de nodos
    std::uniform_int_distribution<int> dist(0, static_cast<int>(vector_nodos.size()) - 1);
    int indice_aleatorio = dist(generador);
    int nodo_actual = vector_nodos[indice_aleatorio];
    // intercambiamos el nodo actual con el último y lo eliminamos para simular el pop
    std::swap(vector_nodos[indice_aleatorio], vector_nodos.back());
    vector_nodos.pop_back();
    if (nodo_actual == arista_fin) {
      encontrado = true;
      break;
    }
    os << std::endl << "-------------------------------------------------------------------------------" << std::endl;
    os << "Iteración " << contador << std::endl;
    os << "Generados: ";
    for (auto& generado : generados) {
      os << generado + 1 << ", ";
    }
    os << std::endl;
    for (int i{0}; i < n_vertices_; ++i) {
      if (!visitado[i] && matr_adyac_[nodo_actual][i] != -1) {
        generados.push_back(i);
        visitado[i] = true;
        padres[i].padre = nodo_actual;
        padres[i].coste = matr_adyac_[nodo_actual][i];
        padres[i].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[i].coste;
        vector_nodos.push_back(i);
      }
    }
    os << "Inspeccionados: ";
    for (auto& inspeccionado : inspeccionados) {
      os << inspeccionado + 1 << ", ";
    }
    inspeccionados.push_back(nodo_actual);
    os << std::endl;
    ++contador;
  }
  os << "-------------------------------------------------------------------------------" << std::endl;
  return encontrado;
}


// en lugar de extraer el siguiente nodo de la cola, se elige entre los 3 mejores nodos de la frontera (los que tienen menos coste)
// se escoge de manera aleatoria entre esos 3 nodos
// modificacion en el BFS

bool Grafo::FronteraAleatoria(int vertice_inicio, int vertice_final, std::vector<Nodo>& padres, std::ostream& os) {
  os << "-------------------------------------------------------------------------------\n";
  os << "Recorrido en Amplitud con frontera aleatoria (top-2 por coste de arista)\n";

  std::mt19937 gen(std::random_device{}());

  // inicializar padres
  padres.resize(n_vertices_);
  for (int i = 0; i < n_vertices_; ++i) {
    padres[i].id = i;
    padres[i].padre = -1;
    padres[i].coste = 0;
    padres[i].coste_acumulado = 0;
  }

  if (vertice_inicio == vertice_final) return true;

  std::vector<bool> visitado(n_vertices_, false);
  visitado[vertice_inicio] = true;

  // estructura de frontera
  struct Candidato {
    int nodo;
    int padre;
    double coste; // coste arista padre -> nodo
  };

  std::vector<Candidato> frontera;
  frontera.reserve(n_vertices_);

  std::vector<int> generados;
  std::vector<int> inspeccionados;
  generados.push_back(vertice_inicio);

  // meter vecinos del inicio en la frontera
  for (int i = 0; i < n_vertices_; ++i) {
    if (!visitado[i] && matr_adyac_[vertice_inicio][i] != -1) {
      visitado[i] = true;
      frontera.push_back({i, vertice_inicio, matr_adyac_[vertice_inicio][i]});

      padres[i].padre = vertice_inicio;
      padres[i].coste = matr_adyac_[vertice_inicio][i];
      padres[i].coste_acumulado = padres[vertice_inicio].coste_acumulado + padres[i].coste;

      generados.push_back(i);
    }
  }

  bool encontrado = false;
  int contador = 1;

  while (!frontera.empty() && !encontrado) {
    os << "\n-------------------------------------------------------------------------------\n";
    os << "Iteración " << contador << "\n";

    os << "Nodos generados: ";
    for (int n : generados) os << n + 1 << ", ";
    os << "\n";

    // buscar los 2 mejores candidatos en la frontera
    int idx1 = -1, idx2 = -1;
    double best1 = frontera[0].coste;
    double best2 = frontera[0].coste;

    for (int idx = 0; idx < static_cast<int>(frontera.size()); ++idx) {
      double c = frontera[idx].coste;
      if (c < best1) {
        best2 = best1;
        idx2 = idx1;

        best1 = c;
        idx1 = idx;
      }
      else if (c < best2) {
        best2 = c;     idx2 = idx;
      }
    }

    // elegir entre los 2 mejores si solo hay uno, se elgie ese
    int idx_elegido = idx1;
    if (idx2 != -1) {
      std::uniform_int_distribution<int> dist(0, 1);
      idx_elegido = (dist(gen) == 0) ? idx1 : idx2;
    }

    Candidato elegido = frontera[idx_elegido];
    // quitar de frontera swap luego pop_back
    std::swap(frontera[idx_elegido], frontera.back());
    frontera.pop_back();

    int nodo_actual = elegido.nodo;

    inspeccionados.push_back(nodo_actual);

    if (nodo_actual == vertice_final) {
      encontrado = true;
      break;
    }

    // expandimos el nodo actual meter sus vecinos en la frontera
    for (int i = 0; i < n_vertices_; ++i) {
      if (!visitado[i] && matr_adyac_[nodo_actual][i] != -1) {
        visitado[i] = true;
        frontera.push_back({i, nodo_actual, matr_adyac_[nodo_actual][i]});

        padres[i].padre = nodo_actual;
        padres[i].coste = matr_adyac_[nodo_actual][i];
        padres[i].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[i].coste;

        generados.push_back(i);
      }
    }

    os << "Nodos inspeccionados: ";
    for (int n : inspeccionados) os << n + 1 << ", ";
    os << "\n";

    ++contador;
  }

  return encontrado;
}


#include <random>
#include <algorithm> // std::swap

bool Grafo::MejorPeor(int vertice_inicio, int vertice_final, std::vector<Nodo>& padres, std::ostream& os) {
  os << "-------------------------------------------------------------------------------\n";
  os << "Recorrido con frontera escogiendo entre el mejor y peor sucesor aleatoriamente\n";

  std::mt19937 generador(std::random_device{}());

  // inicializar padres
  padres.resize(n_vertices_);
  for (int i = 0; i < n_vertices_; ++i) {
    padres[i].id = i;
    padres[i].padre = -1;
    padres[i].coste = 0;
    padres[i].coste_acumulado = 0;
  }

  if (vertice_inicio == vertice_final) return true;

  std::vector<bool> visitado(n_vertices_, false);
  visitado[vertice_inicio] = true;

  // Entrada de frontera: representa "ir desde padre -> nodo" con ese coste
  struct EntradaFrontera {
    int nodo_destino;
    int nodo_origen;
    double coste_arista;
  };

  std::vector<EntradaFrontera> frontera;
  frontera.reserve(n_vertices_);

  std::vector<int> generados;
  std::vector<int> inspeccionados;
  generados.push_back(vertice_inicio);

  // Meter vecinos del inicio
  for (int vecino = 0; vecino < n_vertices_; ++vecino) {
    if (!visitado[vecino] && matr_adyac_[vertice_inicio][vecino] != -1) {
      visitado[vecino] = true;

      frontera.push_back({vecino, vertice_inicio, matr_adyac_[vertice_inicio][vecino]});

      padres[vecino].padre = vertice_inicio;
      padres[vecino].coste = matr_adyac_[vertice_inicio][vecino];
      padres[vecino].coste_acumulado = padres[vertice_inicio].coste_acumulado + padres[vecino].coste;

      generados.push_back(vecino);
    }
  }

  bool encontrado = false;
  int iteracion = 1;

  while (!frontera.empty() && !encontrado) {
    os << "\n-------------------------------------------------------------------------------\n";
    os << "Iteración " << iteracion << "\n";

    os << "Nodos generados: ";
    for (int nodo : generados) os << nodo + 1 << ", ";
    os << "\n";

    // buscar mejor y peor sucesores en la frontera
    int indice_mejor = 0;
    int indice_peor = 0;
    double coste_mejor = frontera[0].coste_arista;
    double coste_peor  = frontera[0].coste_arista;

    for (int i = 1; i < static_cast<int>(frontera.size()); ++i) {
      double coste_actual = frontera[i].coste_arista;

      if (coste_actual < coste_mejor) {
        coste_mejor = coste_actual;
        indice_mejor = i;
      }
      if (coste_actual > coste_peor) {
        coste_peor = coste_actual;
        indice_peor = i;
      }
    }

    // elegir entre mejor y peor
    int indice_elegido = indice_mejor;
    if (indice_peor != indice_mejor) {
      std::uniform_int_distribution<int> dist(0, 1);
      indice_elegido = (dist(generador) == 0) ? indice_mejor : indice_peor;
    }

    EntradaFrontera elegido = frontera[indice_elegido];
    // eliminamos de la frontera
    std::swap(frontera[indice_elegido], frontera.back());
    frontera.pop_back();

    int nodo_actual = elegido.nodo_destino;

    inspeccionados.push_back(nodo_actual);

    if (nodo_actual == vertice_final) {
      encontrado = true;
      break;
    }

    // expandimos nodo_actual
    for (int vecino = 0; vecino < n_vertices_; ++vecino) {
      if (!visitado[vecino] && matr_adyac_[nodo_actual][vecino] != -1) {
        visitado[vecino] = true;

        frontera.push_back({vecino, nodo_actual, matr_adyac_[nodo_actual][vecino]});

        padres[vecino].padre = nodo_actual;
        padres[vecino].coste = matr_adyac_[nodo_actual][vecino];
        padres[vecino].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[vecino].coste;

        generados.push_back(vecino);
      }
    }

    os << "Nodos inspeccionados: ";
    for (int nodo : inspeccionados) os << nodo + 1 << ", ";
    os << "\n";

    ++iteracion;
  }

  return encontrado;
}
