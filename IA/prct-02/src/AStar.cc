/**
 * @file AStar.cc
 * @brief Implementación de la clase AStar para la búsqueda de caminos utilizando el algoritmo A*.
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#include "AStar.h"
#include <vector>
#include <cstdlib>  // para std::abs
#include <algorithm> // para buscar dentro del vector, reemplzar, etc

/**
 * @brief Constructor de la clase AStar.
 * @param laberinto Puntero al laberinto que se va a resolver.
 */
AStar::AStar(const Laberinto& laberinto) : laberinto_(laberinto) {}

/**
 * @brief Calcula la heurística (distancia estimada) entre dos coordenadas.
 * @param coord_actual Coordenada actual.
 * @param coord_final Coordenada objetivo.
 */
int AStar::CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const {
  // Usamos la distancia Manhattan * 3 como heurística
  return (std::abs(coord_final.fila - coord_actual.fila) + std::abs(coord_final.columna - coord_actual.columna)) * 3;
}

/**
 * @brief Calcula el coste de movimiento entre dos coordenadas adyacentes. (7 para diagonal,
 * 5 para horizontal/vertical)
 * @param desde Coordenada de origen.
 * @param hasta Coordenada de destino.
 */
int AStar::CalculaCosteMovimiento (const Coordenada& desde, const Coordenada& hasta) const {
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


/**
 * @brief Busca el camino desde la coordenada de inicio hasta la meta utilizando el algoritmo A*.
 * @param inicio Coordenada de inicio.
 * @return true si se encuentra un camino, false en caso contrario.
 */
bool AStar::BuscarCamino(const Coordenada& inicio) {
  // limpiamos las estructuras por si se llama varias veces a BuscarCamino con el laberinto modificado
  camino_.clear();
  // limpiamos las matrices de nodos en abiertos y cerrados
  nodos_abiertos_.clear();
  nodos_cerrados_.clear();

  // Obtenemos las filas, las columnas, la meta y preparamos las matrices de nodos en abiertos y cerrados
  const int filas = laberinto_.GetFilas();
  const int columnas = laberinto_.GetColumnas();
  nodos_en_cerrados_.assign(filas, std::vector<bool>(columnas, false));
  nodos_en_abiertos_.assign(filas, std::vector<bool>(columnas, false));
  meta_ = laberinto_.ObtenerFin();

  // Caso trivial donde estamos ya en la meta:
  if (inicio.fila == meta_.fila && inicio.columna == meta_.columna) {
    camino_.push_back(inicio);
    return true;
  }
  // Inicializamos el nodo inicial y lo añadimos a la lista de nodos abiertos
  Nodo nodo_inicial;
  nodo_inicial.posicion = inicio;
  nodo_inicial.coste = 0;                   // el coste desde el inicio hasta el inicio es 0
  nodo_inicial.heuristica = CalculaHeuristica(inicio, meta_);
  nodo_inicial.total = nodo_inicial.coste + nodo_inicial.heuristica;
  nodo_inicial.padre = {-1, -1};            // el nodo inicial no tiene padre, se pone como -1, -1
  nodos_abiertos_.push_back(nodo_inicial);  // añadimos el nodo inicial a la lista de abiertos
  nodos_en_abiertos_[inicio.fila][inicio.columna] = true; // marcamos en la matriz booleana que está en abiertos

  bool camino_encontrado{false};
  // Realizamos el bucle mientras haya nodos en abiertos.
  while (!nodos_abiertos_.empty()) {
    // Extraemos el nodo con menor coste total de la lista de nodos abiertos y luego lo marcamos en nodos_cerrados 
    auto it = std::min_element(nodos_abiertos_.begin(), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; });
    if (it != nodos_abiertos_.end()) {    // debería ser siempre cierto ya que nodos_abiertos_ no está vacío si estamos en este punto
      // obtenemos el nodo actual del puntero it
      Nodo nodo_actual = *it;
      // marcamos el nodo como true en nodos_en_cerrados
      nodos_en_cerrados_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = true;
      // marcamos el nodo como false en nodos_en_abiertos
      nodos_en_abiertos_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = false;
      // movemos el nodo de abiertos a cerrados y lo borramos de abiertos
      nodos_cerrados_.push_back(nodo_actual);
      nodos_abiertos_.erase(it);
      // comprobamos si hemos llegado a la meta, y en tal caso reconstruimos el camino y salimos del bucle
      if (nodo_actual.posicion.fila == meta_.fila && nodo_actual.posicion.columna == meta_.columna) {
        camino_encontrado = true;
        ReconstruirCamino(meta_);
        break;
      }
      // exploramos los nodos adyacentes al nodo actual, con los 8 movimientos posibles
      for (int i{0}; i < 8; ++i) {
        // calculamos las coordenadas del nodo adyacente
        Coordenada coordenada_nodo_adyacente;
        coordenada_nodo_adyacente.fila = nodo_actual.posicion.fila + movimientos[i].first;
        coordenada_nodo_adyacente.columna = nodo_actual.posicion.columna + movimientos[i].second;
        // realizamos comprobaciones básicas para saber antes de nada si es una posición valida
        // si no queda dentro del rango, seguimos con el siguiente movimiento
        if (!EnRango(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) {
          continue;
        }
        // si no es transitable, pasamos al siguiente movimiento
        if (!laberinto_.EsTransitable(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) {
          continue;
        }
        // si el nodo adyacente ya está en cerrados, pasamos al siguiente movimiento
        if (nodos_en_cerrados_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) {
          continue;
        }
        // llegados aquí, procedemos a calcular el coste de llegar a ese nodo adyacente
        int coste_tentativo = nodo_actual.coste + CalculaCosteMovimiento(nodo_actual.posicion, coordenada_nodo_adyacente);
        // si el nodo adyacente no está en abiertos, lo añadimos
        if (!nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) {
          // creamos el nodo adyacente, calculamos sus valores y lo añadimos a abiertos
          Nodo nodo_adyacente;
          nodo_adyacente.posicion = coordenada_nodo_adyacente;
          nodo_adyacente.coste = coste_tentativo;
          nodo_adyacente.heuristica = CalculaHeuristica(coordenada_nodo_adyacente, meta_);
          nodo_adyacente.total = nodo_adyacente.coste + nodo_adyacente.heuristica;
          nodo_adyacente.padre = nodo_actual.posicion;
          nodos_abiertos_.push_back(nodo_adyacente);
          // marcamos en la matriz que el nodo está en abiertos
          nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna] = true;
        }
        // Si el vecino ya está en abiertos, comparamos el coste que tiene actualmente y el coste
        // desde el nodo actual, para ver si mejora. si mejora se actualiza.
        else {
          // buscamos el nodo en abiertos y comparamos costes
          auto it2 = std::find_if(nodos_abiertos_.begin(), nodos_abiertos_.end(), [&coordenada_nodo_adyacente](const Nodo& nodo) { return nodo.posicion.fila == coordenada_nodo_adyacente.fila && nodo.posicion.columna == coordenada_nodo_adyacente.columna; });
          if (it2 != nodos_abiertos_.end()) {   // debería ser siempre cierto ya que hemos comprobado que está en abiertos
            if (coste_tentativo < it2->coste) { // si el coste tentativo es menor, actualizamos los valores del nodo en la lista de abiertos
              it2->coste = coste_tentativo;
              it2->total = it2->coste + it2->heuristica;    // la heuristica no cambia asi que se puede usar la que ya tiene
              it2->padre = nodo_actual.posicion;            // actualizamos el padre
            }

          }
        }
      }
    }
  }
  // si salimos del while porque se vacía la lista, es porque no hay camino posible
  return camino_encontrado;

}

/**
 * @brief Reconstruye el camino desde la meta hasta el inicio utilizando los nodos cerrados.
 * @param coord_final Coordenada de la meta.
 */
void AStar::ReconstruirCamino(const Coordenada& coord_final) {
  // El camino está almacenado en el vector nodos_cerrados_ (Con la estructura Nodo tenemos la coordenada del padre)
  // Iré buscando ahí, desde el nodo final, reconstruyendo el camino con su padre.
  Coordenada coordenada_actual = meta_;     // empezamos desde la meta
  int contador = 0;                         // Para poner el coste del camino (está en la primera iteración en el nodo final)
  // establecimos que el nodo inicial tiene como padre las coordenadas -1,-1 así que al llegar ahí paramos
  while (coordenada_actual.fila != -1 && coordenada_actual.columna != -1) {
    camino_.push_back(coordenada_actual);   // añadimos la coordenada actual al camino
    // buscamos el nodo actual en nodos_cerrados_ para obtener su padre
    auto it = std::find_if(nodos_cerrados_.begin(), nodos_cerrados_.end(), [&coordenada_actual](const Nodo& nodo) { return nodo.posicion.fila == coordenada_actual.fila && nodo.posicion.columna == coordenada_actual.columna; });
    // Ahora actualizamos la coordenada_actual al padre del nodo encontrado
    coordenada_actual = it->padre;
    if (contador == 0) {
      coste_ = it->coste; // el coste del camino es el coste del nodo final
    }
    contador++;
  }
  // invertimos el vector camino_ para tenerlo desde el inicio hasta la meta para usarlo en el main dinámico
  std::reverse(camino_.begin(), camino_.end());
}

/**
 * @brief Comprueba si una coordenada está dentro del rango del laberinto.
 * @param fila Fila de la coordenada.
 * @param columna Columna de la coordenada.
 * @return true si la coordenada está dentro del rango, false en caso contrario.
 */
bool AStar::EnRango(int fila, int columna) const {
  bool en_rango = true;
  if (fila < 0 || fila >= laberinto_.GetFilas() || columna < 0 || columna >= laberinto_.GetColumnas()) {
    en_rango = false;
  }
  return en_rango;
}