#include "AStar.h"


AStar::AStar(Laberinto* laberinto) {
  laberinto_ = laberinto;
  // Extraemos el nodo del que partimos para la heuristica y creamos un objeto Nodo:
  Coordenada coordenada_nodo = laberinto_->ObtenerInicio();
  Nodo nodo_inicio;
  nodo_inicio.posicion = coordenada_nodo;
  nodo_inicio.coste = 0;
  nodo_inicio.heuristica = CalculaHeuristica(coordenada_nodo, laberinto_->ObtenerFin());
  nodo_inicio.total = nodo_inicio.coste + nodo_inicio.heuristica;
  nodo_inicio.padre = {-1, -1}; // No tiene padre porque es el nodo inicial
}

int AStar::CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const {
  // Usamos la distancia Manhattan * 3 como heurística
  return (abs(coord_final.fila - coord_actual.fila) + abs(coord_final.columna - coord_actual.columna)) * 3;
}