#include <iostream>
#include "auxiliares.h"
#include "laberinto.h"


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
          os << "■ ";
          break;
        default:   // pintamos lo que está libre en color gris (destaca menos)
          os << mapa[i][j] << " ";
          break;
      }
    }
    os << std::endl;
  }
}

void ModificarEntradaSalida (Laberinto& laberinto, std::ostream& os) {
  std::cout << "Introduzca las nuevas coordenadas de entrada y salida en formato: \"fila, columna\"" << std::endl;
  std::cout << "Tener en cuenta que el laberinto es de dimensión: (" << laberinto.GetFilas() << " x " << laberinto.GetColumnas() << ")" << std::endl;
  int fila, columna;
  std::cout << "Coordenadas de entrada (fila, columna): ";
  std::cin >> fila;
  std::cin.ignore(); // ignorar la coma
  std::cin >> columna;
  if (!laberinto.ModificarEntradaSalida(fila, columna, 0)) {
    std::cerr << "Coordenadas de entrada no válidas." << std::endl;
    return;
  }
  std::cout << "Coordenadas de salida (fila, columna): ";
  std::cin >> fila;
  std::cin.ignore(); // ignorar la coma 
  std::cin >> columna;
  if (!laberinto.ModificarEntradaSalida(fila, columna, 1)) {
    std::cerr << "Coordenadas de salida no válidas." << std::endl;
    return;
  }
  os << "Laberinto actualizado:\n" << laberinto << std::endl;

}