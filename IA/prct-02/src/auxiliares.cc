#include <iostream>
#include "auxiliares.h"
#include "laberinto.h"


void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os, const std::vector<Coordenada>& recorrido_dinamico) {
  std::vector<std::vector<int>> mapa{laberinto.GetMapa()};
  std::vector<std::vector<int>> mapa_{laberinto.GetMapa()};
  // Marcamos el recorrido dinámico si se ha proporcionado
  // Creamos una primera capa, marcando el recorrido actual, que sería el recorrido dinámico
  // con "7" (va cambiando a medida que se añaden coordenadas a recorrido_dinamico)
  for (const auto& coord : recorrido_dinamico) {
    if (mapa[coord.fila][coord.columna] != 3 && mapa[coord.fila][coord.columna] != 4) {  // no sobreescribimos ni la entrada ni la salida
      mapa[coord.fila][coord.columna] = 7; // Marcamos el recorrido dinámico con un valor distinto (7)
    }
  }
  // Esto es una segunda capa, que marca el recorrido planificado, que sería el recorrido del A* hasta el final
  for (const auto& coordenada : recorrido.GetCamino()) {
    if (mapa[coordenada.fila][coordenada.columna] != 3 && mapa[coordenada.fila][coordenada.columna] != 4) {  // no sobreescribimos ni la entrada ni la salida
      mapa[coordenada.fila][coordenada.columna] = 8; // Marcamos el camino encontrado con un valor distinto (8)
    }
  }
  for (size_t i = 0; i < mapa.size(); ++i) {
    for (size_t j = 0; j < mapa[i].size(); ++j) {
      const int v = mapa[i][j];
      if (v == 8) {
        os << ". ";
      }
      else if (v == 7) {
        os << "■ ";
      }
      else {
        os << v << ' ';
      }
    }
    os << '\n';
  }

  os << "\nCoste del camino: " << recorrido.GetCoste() << std::endl;
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

bool RecorridoDinamico(AStar& recorrido, Laberinto& laberinto, std::ostream& os) {
  Coordenada pos_actual = laberinto.ObtenerInicio();
  const Coordenada destino = laberinto.ObtenerFin();
  std::vector<Coordenada> recorrido_real;   // vector donde almacenaremos el recorrido real para imprimir
  int reintentos = 0;
  bool encontrado = true;
  while (true) {
    // planificamos el recorrido sobre el mapa congelado
    AStar a_star(&laberinto);
    if (!a_star.BuscarCamino(pos_actual)) {   // en caso de no encontrar camino
      ++reintentos;
      if (reintentos > 5) {
        os << "Destino inalcanzable con 5 reorganizaciones de obstáculos" << std::endl;
        encontrado = false;
        break;
      }

      // Actualizamos el laberinto
      laberinto.ActualizaObstaculos(0.5, 0.5);  // actualizamos obstáculos
      continue;  // volvemos a intentar planificar el camino
    }
    reintentos = 0;  // si hemos encontrado camino, reiniciamos los reintentos

    // Imprimimos el laberinto con el camino encontrado en el instante actual
    os << "Laberinto con el camino planificado:\n";
    ImprimeCamino(a_star, laberinto, os, recorrido_real);

    // obtenemos el camino planificado
    const auto& camino = a_star.GetCamino();
    // comprobamos si hemos llegado al destino
    if (camino.size() <= 1) {
      os << "Camino encontrado" << std::endl;
      break;
    }
    // avanzamos una posición en el camino. dado que el camino está al revés, la posición es [1]
    // al seleccionar así, siempre cogemos el siguiente nodo del laberinto en nuestro camino
    Coordenada siguiente_nodo = camino[1];    
    recorrido_real.push_back(siguiente_nodo);
    pos_actual = siguiente_nodo;
    // Actualizamos el laberinto (movemos obstáculos)
    laberinto.ActualizaObstaculos(0.5, 0.5);
    // Se repite todo el proceso
  }
  return encontrado;
}