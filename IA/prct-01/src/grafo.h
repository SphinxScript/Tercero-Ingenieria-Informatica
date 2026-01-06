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

/**
 * @brief Estructura para almacenar la información de cada nodo
 */
struct Nodo{
  int id;
  int padre;
  double coste;
  double coste_acumulado;
};

/**
 * @brief Clase para representar un grafo mediante una matriz de adyacencia
 */
class Grafo {
 public:
  Grafo();
  Grafo(std::ifstream&);
  bool RecorridoProfundidad(std::pair<int, int>, std::vector<Nodo>&, std::ostream&);
  bool RecorridoAmplitud(std::pair<int, int>, std::vector<Nodo>&, std::ostream&);
  auto GetMatrix() const { return matr_adyac_; }
  int GetVertices() const { return n_vertices_; }
  int GetAristas() const { return n_aristas_; }

 private:
  std::vector<std::vector<double>> matr_adyac_;
  int n_vertices_;
  int n_aristas_;

  void BuildGraph(std::ifstream&);
  bool Dfs(int, int, std::vector<Nodo>&, std::ostream&);
  bool Bfs(int, int, std::vector<Nodo>&, std::ostream&);

  bool RecorridoAleatorio(int, int, std::vector<Nodo>&, std::ostream&);

};

#endif