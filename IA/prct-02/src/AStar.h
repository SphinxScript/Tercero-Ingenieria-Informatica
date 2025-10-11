#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
//#include <queue>


#include "laberinto.h"

struct Nodo {
  Coordenada posicion; // Coordenada del nodo actual
  int coste; // Coste acumulado desde el nodo inicial
  int heuristica; // Coste estimado hasta el nodo objetivo
  int total; // Suma de coste y heurística
  Coordenada padre; // Coordenada del nodo padre
};

class AStar {
 public:
  AStar() = default;
  AStar(Laberinto* laberinto);
  bool BuscarCamino();
  const std::vector<Coordenada>& GetCamino() const { return camino_; }
 private:
  int CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const;
  int CalculaF(const Coordenada& coord_actual, const Coordenada& coord_final) const;
  int CalculaG(const Coordenada& coord_actual, const Coordenada& coord_vecino) const;
  void ReconstruirCamino(const Coordenada& coord_final);
  Laberinto* laberinto_;
  std::vector<Coordenada> camino_;
  std::vector<Nodo> nodos_abiertos_;
  std::vector<Nodo> nodos_cerrados_;
};

#endif