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


#include <iostream> // para debug (nodos generados e inspeccionados)
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
  camino_.clear();
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
  nodo_inicial.coste = 0;
  nodo_inicial.heuristica = CalculaHeuristica(inicio, meta_);
  nodo_inicial.total = nodo_inicial.coste + nodo_inicial.heuristica;
  nodo_inicial.padre = {-1, -1};
  nodos_abiertos_.push_back(nodo_inicial);
  nodos_en_abiertos_[inicio.fila][inicio.columna] = true;

  int nodos_generados{0};
  int nodos_inspeeccionados{0};

  bool camino_encontrado{false};
  // Realizamos el bucle mientras haya nodos en abiertos.
  while (!nodos_abiertos_.empty()) {
    // Extraemos el nodo con menor coste total
    auto it = std::min_element(nodos_abiertos_.begin(), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; });
    if (it != nodos_abiertos_.end()) {
      Nodo nodo_actual = *it;
      nodos_en_cerrados_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = true;
      nodos_en_abiertos_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = false;
      nodos_cerrados_.push_back(nodo_actual);
      nodos_abiertos_.erase(it);
      // comprobamos si hemos llegado a la meta
      if (nodo_actual.posicion.fila == meta_.fila && nodo_actual.posicion.columna == meta_.columna) {
        camino_encontrado = true;
        ReconstruirCamino(meta_);
        break;
      }
      ++nodos_inspeeccionados;
      // exploramos los nodos adyacentes (8 direcciones)
      for (int i{0}; i < 8; ++i) {
        Coordenada coordenada_nodo_adyacente;
        coordenada_nodo_adyacente.fila = nodo_actual.posicion.fila + movimientos[i].first;
        coordenada_nodo_adyacente.columna = nodo_actual.posicion.columna + movimientos[i].second;
        if (!EnRango(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) {
          continue;
        }
        if (!laberinto_.EsTransitable(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) {
          continue;
        }
        if (nodos_en_cerrados_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) {
          continue;
        }
        // llegados aquí, procedemos a calcular el coste de llegar a ese nodo adyacente (y tenemos un nodo generado)
        ++nodos_generados;
        int coste_tentativo = nodo_actual.coste + CalculaCosteMovimiento(nodo_actual.posicion, coordenada_nodo_adyacente);
        // si el nodo adyacente no está en abiertos, lo añadimos
        if (!nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) {
          Nodo nodo_adyacente;
          nodo_adyacente.posicion = coordenada_nodo_adyacente;
          nodo_adyacente.coste = coste_tentativo;
          nodo_adyacente.heuristica = CalculaHeuristica(coordenada_nodo_adyacente, meta_);
          nodo_adyacente.total = nodo_adyacente.coste + nodo_adyacente.heuristica;
          nodo_adyacente.padre = nodo_actual.posicion;
          nodos_abiertos_.push_back(nodo_adyacente);
          nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna] = true;
        }
        // Si el vecino ya está en abiertos, comparamos el coste que tiene actualmente y el coste
        // desde el nodo actual, para ver si mejora. si mejora se actualiza.
        else {
          auto it2 = std::find_if(nodos_abiertos_.begin(), nodos_abiertos_.end(), [&coordenada_nodo_adyacente](const Nodo& nodo) { return nodo.posicion.fila == coordenada_nodo_adyacente.fila && nodo.posicion.columna == coordenada_nodo_adyacente.columna; });
          if (it2 != nodos_abiertos_.end()) {
            // si el coste tentativo es menor, actualizamos los valores del nodo en la lista de abiertos
            if (coste_tentativo < it2->coste) {
              it2->coste = coste_tentativo;
              it2->total = it2->coste + it2->heuristica;    // la heuristica no cambia asi que se puede usar la que ya tiene
              it2->padre = nodo_actual.posicion;
            }

          }
        }
      }
    }
  }
  // si salimos del while porque se vacía la lista, es porque no hay camino posible
  std::cout << "Nodos generados: " << nodos_generados << std::endl;
  std::cout << "Nodos inspeccionados: " << nodos_inspeeccionados << std::endl;
  return camino_encontrado;

}

/**
 * @brief Reconstruye el camino desde la meta hasta el inicio utilizando los nodos cerrados.
 * @param coord_final Coordenada de la meta.
 */
void AStar::ReconstruirCamino(const Coordenada& coord_final) {
  // El camino está almacenado en el vector nodos_cerrados_ (Con la estructura Nodo tenemos la coordenada del padre)
  Coordenada coordenada_actual = meta_;
  int contador = 0;
  while (coordenada_actual.fila != -1 && coordenada_actual.columna != -1) {
    camino_.push_back(coordenada_actual);
    auto it = std::find_if(nodos_cerrados_.begin(), nodos_cerrados_.end(), [&coordenada_actual](const Nodo& nodo) { return nodo.posicion.fila == coordenada_actual.fila && nodo.posicion.columna == coordenada_actual.columna; });
    coordenada_actual = it->padre;
    if (contador == 0) {
      coste_ = it->coste;
    }
    contador++;
  }
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