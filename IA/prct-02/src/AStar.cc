#include "AStar.h"
#include <vector>
#include <cstdlib>  // para std::abs
#include <algorithm> // para buscar dentro del vector, reemplzar, etc

AStar::AStar(const Laberinto* laberinto) : laberinto_(laberinto) {}

int AStar::CalculaHeuristica(const Coordenada& coord_actual, const Coordenada& coord_final) const {
  // Usamos la distancia Manhattan * 3 como heurística
  return (std::abs(coord_final.fila - coord_actual.fila) + std::abs(coord_final.columna - coord_actual.columna)) * 3;
}

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


// Desarrollamos el bucle while para buscar el camino
bool AStar::BuscarCamino(const Coordenada& inicio) {
  camino_.clear();
  nodos_abiertos_.clear();
  nodos_cerrados_.clear();

  const int filas = laberinto_->GetFilas();
  const int columnas = laberinto_->GetColumnas();
  nodos_en_cerrados_.assign(filas, std::vector<bool>(columnas, false));
  nodos_en_abiertos_.assign(filas, std::vector<bool>(columnas, false));
  meta_ = laberinto_->ObtenerFin();

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
  nodo_inicial.padre = {-1, -1}; // el nodo inicial no tiene
  nodos_abiertos_.push_back(nodo_inicial);
  nodos_en_abiertos_[inicio.fila][inicio.columna] = true; // marcamos en la matriz que está en abiertos

  bool camino_encontrado{false};
  while (!nodos_abiertos_.empty()) {
    // Extraemos el nodo con menor coste total de la lista de nodos abiertos y luego lo marcamos en nodos_cerrados 
    auto it = std::min_element(nodos_abiertos_.begin(), nodos_abiertos_.end(), [](const Nodo& a, const Nodo& b) { return a.total < b.total; });
    if (it != nodos_abiertos_.end()) {    // debería ser siempre cierto
      Nodo nodo_actual = *it;
      // marcamos el nodo como true en nodos_en_cerrados
      nodos_en_cerrados_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = true;
      // marcamos el nodo como false en nodos_en_abiertos
      nodos_en_abiertos_[nodo_actual.posicion.fila][nodo_actual.posicion.columna] = false;
      // movemos el nodo de abiertos a cerrados
      nodos_cerrados_.push_back(nodo_actual);
      nodos_abiertos_.erase(it);
      // comprobamos si hemos llegado a la meta
      if (nodo_actual.posicion.fila == meta_.fila && nodo_actual.posicion.columna == meta_.columna) {
        camino_encontrado = true;
        ReconstruirCamino(meta_);
        break;
      }
      for (int i{0}; i < 8; ++i) {    // vamos a recorrer todos los vecinos del nodo actual (con su posición) en el vector movimientos (tamaño 8)
        Coordenada coordenada_nodo_adyacente;
        coordenada_nodo_adyacente.fila = nodo_actual.posicion.fila + movimientos[i].first;
        coordenada_nodo_adyacente.columna = nodo_actual.posicion.columna + movimientos[i].second;
        // realizamos comprobaciones básicas para saber antes de nada si es una posición valida
        if (!EnRango(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) {   // si no queda dentro del rango, seguimos con el siguiente movimiento
          continue; // si no está en rango, pasamos al siguiente movimiento
        }
        if (!laberinto_->EsTransitable(coordenada_nodo_adyacente.fila, coordenada_nodo_adyacente.columna)) { // si no es transitable, pasamos al siguiente movimiento
          continue;
        }
        if (nodos_en_cerrados_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) { // si ya está en nodos cerrados, pasamos al siguiente movimiento
          continue;
        }
        // Procedemos a calcular el coste tentativo de llegar a ese nodo adyacenta
        int coste_tentativo = nodo_actual.coste + CalculaCosteMovimiento(nodo_actual.posicion, coordenada_nodo_adyacente);
        // Si el nodo adyacente no está en abiertos, comprobamos si el nuevo coste es menor que el coste actual
        if (!nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna]) {
          // Si el nodo adyacente no está en abiertos, lo añadimos
          Nodo nodo_adyacente;
          nodo_adyacente.posicion = coordenada_nodo_adyacente;
          nodo_adyacente.coste = coste_tentativo;
          nodo_adyacente.heuristica = CalculaHeuristica(coordenada_nodo_adyacente, meta_);
          nodo_adyacente.total = nodo_adyacente.coste + nodo_adyacente.heuristica;
          nodo_adyacente.padre = nodo_actual.posicion;
          nodos_abiertos_.push_back(nodo_adyacente);
          nodos_en_abiertos_[coordenada_nodo_adyacente.fila][coordenada_nodo_adyacente.columna] = true; // marcamos en la matriz que está en abiertos
        }
        // Si el vecino ya está en abiertos, comparamos el coste que tiene y el coste desde el nodo actual, para ver si mejora. si mejora se actualiza.
        else {
          // buscamos el nodo en nodos_abiertos_ y lo ponemos como iterador
          auto it2 = std::find_if(nodos_abiertos_.begin(), nodos_abiertos_.end(), [&coordenada_nodo_adyacente](const Nodo& nodo) { return nodo.posicion.fila == coordenada_nodo_adyacente.fila && nodo.posicion.columna == coordenada_nodo_adyacente.columna; });
          if (it2 != nodos_abiertos_.end()) { // debería ser siempre cierto ya que hemos comprobado que está en abiertos
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

// este metodo calcula el camino e inserta en el vector<Coordenada> las coordenadas del camino
void AStar::ReconstruirCamino(const Coordenada& coord_final) {
  // El camino está almacenado en el vector nodos_cerrados_
  // Iré buscando ahí, desde el nodo final, reconstruyendo el camino con su padre.
  Coordenada coordenada_actual = meta_;
  int contador = 0;   // Para poner el coste del camino (almacenado el total en el nodo final)
  while (coordenada_actual.fila != -1 && coordenada_actual.columna != -1) { // establecimos que el nodo inicial tiene como padre las coordenadas -1,-1
    camino_.push_back(coordenada_actual);
    auto it = std::find_if(nodos_cerrados_.begin(), nodos_cerrados_.end(), [&coordenada_actual](const Nodo& nodo) { return nodo.posicion.fila == coordenada_actual.fila && nodo.posicion.columna == coordenada_actual.columna; });
    // Ahora establecemos la coordenada actual al padre
    coordenada_actual = it->padre;
    if (contador == 0) {
      coste_ = it->coste; // el coste del camino es el coste del nodo final
    }
    contador++;
  }
  std::reverse(camino_.begin(), camino_.end()); // dado que hemos ido del final al inicio, invertimos el vector para tener el camino en orden
}


bool AStar::EnRango(int fila, int columna) const {
  bool en_rango = true;
  if (fila < 0 || fila >= laberinto_->GetFilas() || columna < 0 || columna >= laberinto_->GetColumnas()) {
    en_rango = false;
  }
  return en_rango;
}