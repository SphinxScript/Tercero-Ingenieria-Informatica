#ifndef AUX_h
#define AUX_H

#include "AStar.h"
#include "laberinto.h"

void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os);
void ModificarEntradaSalida(Laberinto& laberinto);

#endif