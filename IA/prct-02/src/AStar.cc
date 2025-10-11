#include "AStar.h"
#include <vector>
#include <cstdlib>  // para std::abs
#include <algorithm> // para buscar dentro del vector, reemplzar, etc

AStar::AStar(Laberinto* laberinto) {
  laberinto_ = laberinto;
  // Extraemos el nodo del que partimos para la heuristica y creamos un objeto Nodo:
  Coordenada coordenada_nodo = laberinto_->ObtenerInicio();
  Nodo nodo_inicio;
  nodo_inicio.posicion = coordenada_nodo;
  nodo_inicio.coste = 0; // porque es el nodo inicial
  nodo_inicio.heuristica = CalculaHeuristica(coordenada_nodo, laberinto_->ObtenerFin());
  nodo_inicio.total = nodo_inicio.coste + nodo_inicio.heuristica;
  nodo_inicio.padre = {-1, -1}; // No tiene padre porque es el nodo inicial

  nodos_abiertos_.push_back(nodo_inicio); // Añadimos el nodo inicial a la lista de nodos abiertos
}

int AStar::CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const {
  // Usamos la distancia Manhattan * 3 como heurística
  return (std::abs(coord_final.fila - coord_actual.fila) + std::abs(coord_final.columna - coord_actual.columna)) * 3;
}

int CalculaCosteMovimiento (const Coordenada& desde, const Coordenada& hasta) {
  // si el movimiento es horizontal o vertical (solo cambia el índice de la columna ó la fila (no ambos)) el coste es 5
  // en caso de cambiar ambos (movimiento diagonal) el coste es 7
  // no consideramos movimientos nulos (me quedo en la misma coordenada)
  int coste{0};
  if ((desde.fila == hasta.fila && desde.columna != hasta.columna) || (desde.columna == hasta.columna && desde.fila != hasta.fila)) {
    coste = 5;
  }
  else if (desde.fila != hasta.fila && desde.columna != hasta.columna) {
    coste = 7;
  }
  return coste;
}

bool AStar::BuscarCamino() {
  bool camino_encontrado{false};
  while (!nodos_abiertos_.empty()) {
    auto it = std::min_element(nodos_abiertos_.begin(), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; });
    if (it != nodos_abiertos_.end()) {    // debería ser siempre cierto
      Nodo nodo_actual = *it;
      nodos_cerrados_.push_back(nodo_actual);
      nodos_abiertos_.erase(it);
      if (nodo_actual.posicion.fila == laberinto_->ObtenerFin().fila && nodo_actual.posicion.columna == laberinto_->ObtenerFin().columna) {
        camino_encontrado = true;
        break;
      }
      for (int i{0}; i < 8; ++i) {    // vamos a recorrer todos los vecinos del nodo actual (con su posición) en el vector movimientos (tamaño 8)
        Coordenada coordenada_nodo_adyacente;
        coordenada_nodo_adyacente.fila = nodo_actual.posicion.fila + movimientos[i].first;
        coordenada_nodo_adyacente.columna = nodo_actual.posicion.columna + movimientos[i].second;
        // realizamos comprobaciones básicas para saber antes de nada si es una posición valida
        if ()
      }
    }
  }

}