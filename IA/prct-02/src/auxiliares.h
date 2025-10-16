#ifndef AUX_h
#define AUX_H

#include "AStar.h"
#include "laberinto.h"

void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os, const std::vector<Coordenada>& recorrido_dinamico = {});
void ModificarEntradaSalida(Laberinto& laberinto, std::ostream& os);

// Esta función externa se encarga de gestionar el recorrido dinámico
// del laberinto, actualizando los obstáculos y recalculando el camino
// en cada iteración. Devuelve true si se encuentra un camino hasta la meta,
// o false si no es posible llegar a la meta.
// imprime en cada iteración
bool RecorridoDinamico(AStar& recorrido, Laberinto& laberinto, std::ostream& os);

inline int PasoCoste(const Coordenada& actual, const Coordenada& siguiente) {
  const bool orto = (actual.fila == siguiente.fila) ^ (actual.columna == siguiente.columna);
  const bool diag = (actual.fila != siguiente.fila) && (actual.columna != siguiente.columna);
  return orto ? 5 : (diag ? 7 : 0);
}


#endif