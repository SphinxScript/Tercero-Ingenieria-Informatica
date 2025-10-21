/**
 * @file AStar.cc
 * @brief Implementación de la clase AStar para la búsqueda de caminos utilizando el algoritmo A*.
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#include "AStar.h"
#include <vector>
#include <cstdlib>  // para std::abs
#include <algorithm> // para buscar dentro del vector, reemplzar, nth_element


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
  // generador de números aleatorios para la selección del nodo en la primera iteración
  static std::mt19937 gen{std::random_device{}()};

  camino_.clear();
  nodos_abiertos_.clear();
  nodos_cerrados_.clear();

  const int filas = laberinto_.GetFilas();
  const int columnas = laberinto_.GetColumnas();
  nodos_en_cerrados_.assign(filas, std::vector<bool>(columnas, false));
  nodos_en_abiertos_.assign(filas, std::vector<bool>(columnas, false));
  meta_ = laberinto_.ObtenerFin();

  if (inicio.fila == meta_.fila && inicio.columna == meta_.columna) {
    camino_.push_back(inicio);
    return true;
  }

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

  // contador de iteraciones
  int iteracion = 0;

  while (!nodos_abiertos_.empty()) {

    int k = 0; // indice para extraer el nodo

    if (iteracion == 1) {   // iteración 1 ya que la 0 solo tiene la casilla inicial
      // primera iteracion: escoger aleatoriamente uno entre los 3 con menor coste
      int limite = std::min(3, static_cast<int>(nodos_abiertos_.size()));
      std::nth_element(nodos_abiertos_.begin(), nodos_abiertos_.begin() + (limite - 1), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; } );
      // generamos un índice aleatorio entre 0 y limite - 1
      std::uniform_int_distribution<int> dist(0, limite - 1);
      k = dist(gen);
    }
    else {
      // con la segunda, escogemos el mejor coste
      auto it_min = std::min_element(nodos_abiertos_.begin(), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; } );
      k = static_cast<int>(std::distance(nodos_abiertos_.begin(), it_min));
    }

    // extraemos el nodo escogido de abiertos y lo borramos
    Nodo nodo_actual = nodos_abiertos_[static_cast<size_t>(k)];
    nodos_abiertos_.erase(nodos_abiertos_.begin() + k);
    nodos_en_abiertos_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = false;

    // lo movemos a cerrados
    nodos_en_cerrados_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = true;
    nodos_cerrados_.push_back(nodo_actual);


    if (nodo_actual.posicion.fila == meta_.fila && nodo_actual.posicion.columna == meta_.columna) {
      camino_encontrado = true;
      ReconstruirCamino(meta_);
      break;
    }

    ++nodos_inspeeccionados;

    for (int i{0}; i < 8; ++i) {
      Coordenada coord_vecino{
        nodo_actual.posicion.fila + movimientos[i].first,
        nodo_actual.posicion.columna + movimientos[i].second
      };

      if (!EnRango(coord_vecino.fila, coord_vecino.columna)) continue;
      if (!laberinto_.EsTransitable(coord_vecino.fila, coord_vecino.columna)) continue;
      if (nodos_en_cerrados_[coord_vecino.fila][coord_vecino.columna]) continue;

      ++nodos_generados;
      int coste_tentativo = nodo_actual.coste + CalculaCosteMovimiento(nodo_actual.posicion, coord_vecino);

      if (!nodos_en_abiertos_[coord_vecino.fila][coord_vecino.columna]) {
        Nodo nodo_ady;
        nodo_ady.posicion = coord_vecino;
        nodo_ady.coste = coste_tentativo;
        nodo_ady.heuristica = CalculaHeuristica(coord_vecino, meta_);
        nodo_ady.total = nodo_ady.coste + nodo_ady.heuristica;
        nodo_ady.padre = nodo_actual.posicion;

        nodos_abiertos_.push_back(nodo_ady);
        nodos_en_abiertos_[coord_vecino.fila][coord_vecino.columna] = true;
      }
      else {
        auto it2 = std::find_if( nodos_abiertos_.begin(), nodos_abiertos_.end(), [&coord_vecino](const Nodo& n) { return n.posicion.fila == coord_vecino.fila && n.posicion.columna == coord_vecino.columna; } );
        if (it2 != nodos_abiertos_.end() && coste_tentativo < it2->coste) {
          it2->coste = coste_tentativo;
          it2->total = it2->coste + it2->heuristica; // h no cambia
          it2->padre = nodo_actual.posicion;
        }
      }
    }

    ++iteracion;
  }

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

// std::nth_element(nodos_abiertos_.begin(), nodos_abiertos_.begin() + 2, nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; })