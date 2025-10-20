/**
 * @file auxiliares.cc
 * @brief Fichero con las funciones auxiliares para la práctica 2 de IA. 3 año, Ingeniería Informática, Universidad de La Laguna
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#include <iostream>
#include "auxiliares.h"
#include "laberinto.h"

/**
 * @brief Función que imprime el laberinto  con solución en un stream de salida
 * @param recorrido Objeto AStar con el recorrido planificado
 * @param laberinto Objeto Laberinto con el mapa
 * @param os Stream de salida
 * @param recorrido_dinamico Vector de Coordenada con el recorrido real realizado hasta el momento (para el recorrido dinámico)
 * @note Si recorrido_dinamico está vacío, no se imprime el recorrido dinámico
 * @return void
 */
void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os, const std::vector<Coordenada>& recorrido_dinamico) {
  // Obtenemos el mapa del laberinto
  std::vector<std::vector<int>> mapa{laberinto.GetMapa()};
  /**
   * Aquí marcamos en el mapa obtenido del laberinto el recorrido dinámico, si es que lo hay
   * con un "7". El recorrido dinámico es el que se va realizando en cada paso del A* donde se recalculan los obstáculos
   * y se va avanzando una posición en el camino planificado.
   * En caso de no haber recorrido dinámico, pues este vector estará vacío y no se marcará nada.
  */
  for (const Coordenada& coord : recorrido_dinamico) {
    if (mapa[coord.fila][coord.columna] != 3 && mapa[coord.fila][coord.columna] != 4) {  // no sobreescribimos ni la entrada ni la salida
      mapa[coord.fila][coord.columna] = 7; // Marcamos el recorrido dinámico con un valor distinto (7)
    }
  }
  /**
   * Aquí marcamos el recorrido planificado con A* con un "8" (si no es la entrada ni la salida)
   * De esta forma, tenemos 2 capas en un mismo mapa:
   * - La capa del recorrido dinámico, que es el recorrido que se va realizando en cada paso del A* (se añade un 7 por cada paso)
   * - La capa del recorrido planificado, que es el recorrido completo que ha planificado
   * Así podemos ver en cada iteración donde va el agente (7) y cual sería el camino total de ese A* en ese momento de tiempo (8)
   */
  for (const Coordenada& coordenada : recorrido.GetCamino()) {
    if (mapa[coordenada.fila][coordenada.columna] != 3 && mapa[coordenada.fila][coordenada.columna] != 4) {  // no sobreescribimos ni la entrada ni la salida
      mapa[coordenada.fila][coordenada.columna] = 8; // Marcamos el camino encontrado con un valor distinto (8)
    }
  }
  /**
   * Aquí imprimimos el mapa con las siguientes convenciones:
   * - 0: casilla libre -> se imprime como "0 "
   * - 1: obstáculo -> se imprime como "1 "
   * - 3: entrada -> se imprime como "E "
   * - 4: salida -> se imprime como "S "
   * - 7: recorrido dinámico -> se imprime como "■ "
   * - 8: recorrido planificado -> se imprime como ". "
   * He dado prioridad al recorrido dinámico (es más visible) sobre el recorrido total planificado.
   * De forma que se ve con un "■" la posición real del agente, y con "." el camino que tomaría en el A* de ese laberinto en ese instante
   */
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
  os << std::endl;

}


/**
 * @brief Función que modifica las coordenadas de entrada y salida del laberinto
 * @param laberinto Objeto Laberinto a modificar
 * @param os Stream de salida que se encarga de imprimir las instrucciones para el usuario
 * @return void
 */
bool ModificarEntradaSalida (Laberinto& laberinto, std::ostream& os) {
  std::cout << "Introduzca las nuevas coordenadas de entrada y salida en formato: \"fila columna\"" << std::endl;
  std::cout << "Tener en cuenta que el laberinto es de dimensión: (" << laberinto.GetFilas() << " x " << laberinto.GetColumnas() << ")" << std::endl;
  int fila, columna;
  std::cout << "Coordenadas de entrada (fila columna): ";
  std::cin >> fila;
  std::cin >> columna;
  // Comprobamos si se ha modificado correctamente la entrada
  if (!laberinto.ModificarEntradaSalida(fila, columna, 0)) {
    std::cerr << "Coordenadas de entrada no válidas." << std::endl;
    return false;
  }
  std::cout << "Coordenadas de salida (fila columna): ";
  std::cin >> fila;
  std::cin >> columna;
  // Comprobamos si se ha modificado correctamente la salida
  if (!laberinto.ModificarEntradaSalida(fila, columna, 1)) {
    std::cerr << "Coordenadas de salida no válidas." << std::endl;
    return false;
  }
  os << "Laberinto actualizado:\n" << laberinto << std::endl;
  return true;
}

/**
 * @brief Función principal de la práctica. Realiza un recorrido dinámico con A* en un laberinto donde los obstáculos
 * se mueven en cada iteración. Nótese por iteración cada "paso" que da el agente en el laberinto. En cada paso, se realiza A* con 
 * los obstáculos recalculados. El % de obstáculos en el laberinto ha de ser < 25% para asegurar la existencia de caminos. Si a pesar de
 * ello no se encuentra camino, se reintenta hasta 5 veces, recalculando el A* en esa posición hasta el momento. Si no se encuentra camino
 * en 5 intentos, se da por inalcanzable la meta.
 * @param recorrido Objeto AStar para realizar el recorrido
 * @param laberinto Objeto Laberinto donde se realiza el recorrido
 * @param os Stream de salida donde se imprime el laberinto y el camino en cada paso
 * @return true si se ha encontrado un camino hasta la meta, false en caso contrario
 */
bool RecorridoDinamico(AStar& recorrido, Laberinto& laberinto, std::ostream& os) {
  Coordenada pos_actual = laberinto.ObtenerInicio();  // Obtenemos la coordenada posición inicial para comenzar A*
  std::vector<Coordenada> recorrido_real;             // vector donde almacenaremos el recorrido real para imprimir
  int reintentos = 0;                 // contador de reintentos al no encontrar camino
  bool encontrado = true;             // variable para indicar si se ha encontrado el camino o no
  int coste_acumulado = 0;            // coste acumulado del recorrido dinámico para ir imprimiendo

  while (true) {        // Bucle principal del recorrido dinámico
    AStar a_star(laberinto);                 // creamos un nuevo A* con el laberinto actualizado actual
    if (!a_star.BuscarCamino(pos_actual)) {   // en caso de no encontrar camino
      ++reintentos;                           // incrementamos el contador de reintentos
      if (reintentos > 5) {                   // si hemos superado los 5 reintentos, damos por inalcanzable la meta
        os << "Destino inalcanzable con 5 reorganizaciones de obstáculos" << std::endl;
        os << "Coste acumulado hasta el momento: " << coste_acumulado << std::endl;
        encontrado = false;
        break;
      }

      laberinto.ActualizaObstaculos(0.5, 0.5);  // actualizamos obstáculos
      continue;                                 // volvemos a intentar planificar el camino
    }
    reintentos = 0;                             // si hemos encontrado camino, reiniciamos los reintentos

    // Imprimimos el laberinto con el camino encontrado en el instante actual y el recorrido que lleva el agente
    os << "Laberinto con el camino planificado:" << std::endl;
    ImprimeCamino(a_star, laberinto, os, recorrido_real);
    os << "Coste acumulado hasta el momento: " << coste_acumulado << std::endl;

    const std::vector<Coordenada>& camino = a_star.GetCamino();    // obtenemos el camino planificado hasta el final
    if (camino.size() <= 1) {                                      // si el tamaño del camino es 1 o menos, hemos llegado a la meta
      os << "¡Destino alcanzado!" << std::endl;
      break;
    }
    
    Coordenada siguiente_nodo = camino[1];    // avanzamos una posición en el camino, que es la posición [1] del vector camino en cada
                                              // iteración dado que éste se recalcula en cada iteración desde la posición actual del agente
    const int coste_paso = PasoCoste(pos_actual, siguiente_nodo); // calculamos el coste del paso actual con la función auxiliar creada
    coste_acumulado += coste_paso;                  // actualizamos el coste acumulado
    recorrido_real.push_back(siguiente_nodo);       // añadimos la nueva posición al recorrido real
    pos_actual = siguiente_nodo;                    // actualizamos la posición actual del agente
    laberinto.ActualizaObstaculos(0.5, 0.5);        // Actualizamos el laberinto (movemos obstáculos)
    // Se repite todo el proceso
  }
  return encontrado;
}