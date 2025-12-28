/**
 * @file grafo.cc
 * @brief fichero para implementar la clase Grafo
 * @author Ricardo David Rodríguez Pane
 * @date 26-12-2025
 */

#include <fstream>
#include <iostream>
#include <stack>

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
  for (int i{0}; i < n_vertices_; ++i) {
    for (int j{i + 1}; j < n_vertices_; ++j) {
      // la matriz en la posición i,j conecta ambos nodos con el peso en el fichero de entrada
      // como es simetrica (grafo no dirigido) también en la posición j, i
      double coste;
      flujo_entrada >> coste;
      matr_adyac_[i][j] = coste;
      matr_adyac_[j][i] = coste;
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
bool Grafo::RecorridoProfundidad(std::pair<int, int> inicio_fin, std::vector<Nodo>& padres) {
  int inicio = inicio_fin.first;
  int fin = inicio_fin.second;
  --inicio;
  --fin;
  return Dfs(inicio, fin, padres);
}

/**
 * @brief Realiza el recorrido en profundidad desde el nodo inicio al nodo fin
 * imprime durante las iteraciones los nodos generados e inspeccionados
 * @param nodo_inicio nodo desde el que se inicia el recorrido
 * @param arista_fin nodo que se quiere alcanzar
 * @param padres vector de nodos que almacena el orden de visita
 * @return true si se encuentra el nodo fin, false en caso contrario
 */
bool Grafo::Dfs(int nodo_inicio, int arista_fin, std::vector<Nodo>& padres) {
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
    std::cout << std::endl << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "Iteración " << contador << std::endl;
    std::cout << "Generados: ";
    for (auto& generado : generados) {
      std::cout << generado + 1 << ", ";
    }
    std::cout << std::endl;
    for (int i{0}; i < n_vertices_; ++i) {
      if (!visitado[i] && matr_adyac_[nodo_actual][i] != -1) {
        generados.push_back(i);
        visitado[i] = true;
        padres[i].padre = nodo_actual;
        padres[i].coste = matr_adyac_[nodo_actual][i];
        padres[i].coste_acumulado = padres[nodo_actual].coste_acumulado + padres[i].coste;
        pila_nodos.push(i);
        if (arista_fin == i) {
          encontrado = true;
          break;
        }
      }
    }
    std::cout << "Inspeccionados: ";
    for (auto& inspeccionado : inspeccionados) {
      std::cout << inspeccionado + 1 << ", ";
    }
    inspeccionados.push_back(nodo_actual);
    std::cout << std::endl;
    ++contador;
  }
  std::cout << "-------------------------------------------------------------------------------" << std::endl;
  return encontrado;
}