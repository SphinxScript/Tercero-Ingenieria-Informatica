#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
//#include <queue>


#include "laberinto.h"

struct Nodo {
  Coordenada posicion; // Coordenada del nodo actual
  int coste; // Coste acumulado desde el nodo inicial
  int heuristica; // Coste estimado hasta el nodo objetivo
  int total; // Suma de coste y heurística -> f(n)
  Coordenada padre; // Coordenada del nodo padre
};

class AStar {
 public:
  AStar() = default;
  AStar(const Laberinto* laberinto);
  bool BuscarCamino();
  const std::vector<Coordenada>& GetCamino() const { return camino_; }
 private:
  int CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const;
  void ReconstruirCamino(const Coordenada& coord_final);
  int CalculaCosteMovimiento(const Coordenada& desde, const Coordenada& hacia) const;
  bool EnRango(int fila, int columna) const;
  const Laberinto* laberinto_;      // un puntero al laberinto que estamos resolviendo
  std::vector<Coordenada> camino_;    // vector donde guardaremos el camino desde el inicio al fin si lo encontramos
  std::vector<Nodo> nodos_abiertos_;  // nodos que están pendientes de explorar de tipo Nodo
  std::vector<Nodo> nodos_cerrados_;  // nodos que ya han sido explorados de tipo Nodo
  std::vector<std::vector<bool>> nodos_en_cerrados_; // matriz para marcar los nodos de la lista cerrados
  std::vector<std::vector<bool>> nodos_en_abiertos_; // matriz para marcar los nodos que están en abiertos
  Coordenada meta_;  // coordenada del nodo objetivo (la meta)

  std::vector<std::pair<int, int>> movimientos{{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
};

#endif