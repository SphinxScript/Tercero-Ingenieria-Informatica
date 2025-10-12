#include <iostream>
#include "auxiliares.h"


void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os) {
  std::vector<std::vector<int>> mapa{laberinto.GetMapa()};
  for (const auto& coordenada : recorrido.GetCamino()) {
    mapa[coordenada.fila][coordenada.columna] = 8;
  }
  for (int i{0}; i < mapa.size(); ++i) {
    for (int j{0}; j < mapa[i].size(); ++j) {
      switch (mapa[i][j]) {
        case 1: // son muros, los pinto de blanco (por defecto)
          os << mapa[i][j] << " ";
          break;
        case 8:   // el camino en color rojo sobre azul claro
          os << "\033[31;104m" << mapa[i][j] << "\033[0m ";
          break;
        default:   // pintamos lo que está libre en color gris (destaca menos)
          os << "\033[90m" << mapa[i][j] << "\033[0m ";
          break;
      }
    }
    os << std::endl;
  }
}