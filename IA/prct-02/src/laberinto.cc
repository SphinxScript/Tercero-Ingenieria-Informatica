#include <fstream>
#include <sstream>
#include <iostream>
#include <random>   // para la generación del número aleatorio
#include <algorithm> // para std::shuffle
#include <chrono>   // para la semilla del generador de números aleatorios
#include "laberinto.h"


Laberinto::Laberinto(const std::string& nombre_fichero) {
  nombre_fichero_ = nombre_fichero;
  const auto t = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::seed_seq seq{ static_cast<uint32_t>(t), static_cast<uint32_t>(t >> 32), static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this))};
  rng_.seed(seq);


  bool cargado = Cargar();
  if (!cargado) {
    std::cerr << "Error al cargar el laberinto desde el fichero" << std::endl;
    filas_ = 0;
    columnas_ = 0;
  }

  std::mt19937 rng(std::random_device{}());              // motor de generación de números aleatorios

}


bool Laberinto::Cargar() {
  std::ifstream fichero{nombre_fichero_};
  if (!fichero.is_open()) {
    std::cerr << "Error al abrir el fichero" << std::endl;
    return false;
  }
  fichero >> filas_ >> columnas_;
  mapa_.resize(filas_, std::vector<int>(columnas_));
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      fichero >> mapa_[i][j];
      if (mapa_[i][j] == 3) {     // si la posición es 3, es el inicio
        inicio_.fila = i;
        inicio_.columna = j;
      }
      else if (mapa_[i][j] == 4) {    // si la posición es 4, es el fin
        fin_.fila = i;
        fin_.columna = j;
      }
    }
  }
  fichero.close();
  return true;
}

bool Laberinto::EsTransitable(int fila, int columna) const {
  bool estransitable = true;
  if (mapa_[fila][columna] == 1) {       // comprobamos que no sea ningún muro u obstáculo (Aquí ya hemos comprobado que está dentro de los limites)
    estransitable = false;
  }
  return estransitable;
}

bool Laberinto::ModificarEntradaSalida(int fila, int columna, int selector) {
  bool operacion = true;
  if (fila < 0 || columna < 0 ||fila >= this->GetFilas() || columna >= this->GetColumnas()) {
    operacion = false;
  }
  else {
    if (selector == 0) {    // Modificamos la entrada
      // Primero establecemos como 0 la entrada anterior:
      this->mapa_[this->inicio_.fila][this->inicio_.columna] = 0;
      // Ahora colocamos el 3 en la nueva entrada
      this->mapa_[fila][columna] = 3;
      // ahora establecemos el valor de la entrada nueva.
      this->inicio_ = Coordenada {fila, columna};
    }
    else {    // si vamos por aqui, es que es la salida
      // Primero establecemos como 0 el final anterior:
      this->mapa_[this->fin_.fila][this->fin_.columna] = 0;
      // Ahora colocamos el 4 en la nueva salida
      this->mapa_[fila][columna] = 4;
      // ahora establecemos el valor de la salida nueva.
      this->fin_ = Coordenada {fila, columna};
    }
  }
  return operacion;
}

void Laberinto::ActualizaObstaculos(double pin, double pout) {
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      if (mapa_[i][j] == 3 || mapa_[i][j] == 4) {   // no modificamos la entrada ni la salida
        continue;
      }
      if (mapa_[i][j] == 0) {  // si está libre
        double probabilidad = uni01_(rng_); // generamos un número aleatorio entre 0 y 1
        if (probabilidad < pin) {   // si el número es menor que pin, lo convertimos en obstáculo
          mapa_[i][j] = 1;   // lo convertimos en obstáculo
        }
      }
      else if (mapa_[i][j] == 1) { // si es un obstáculo
        double probabilidad = uni01_(rng_); // generamos un número aleatorio entre 0 y 1
        if (probabilidad < pout) {    // si el número es menor que pout, lo convertimos en libre
          mapa_[i][j] = 0;   // lo convertimos en libre
        }
      }
      else {
        continue;   // si es cualquier otro valor, no lo modificamos
      }
    }
  }
  // comprobamos si el porcentaje de obstáculos es menor del 25%
  int max_obstaculos = (filas_ * columnas_) / 4;   // 25% del total de casillas. Entero porque la diferencia es pequeña
  if (NumeroObstaculos() >= max_obstaculos) {    // En caso de que haya más del 25%, llamamos a EliminaObstaculosAzar
    EliminaObstaculosAzar(NumeroObstaculos() - max_obstaculos);   // eliminamos el número de obstaculos de la diferencia con el 25%
  }
}

// Esta función devuelve el número de obstáculos en el laberinto
int Laberinto::NumeroObstaculos() const {
  int contador_obstaculos = 0;
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      if (mapa_[i][j] == 1) {
        contador_obstaculos++;
      }
    }
  }
  return contador_obstaculos;
}

void Laberinto::EliminaObstaculosAzar(int a_liberar) {
  std::vector<Coordenada> obstáculos;
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      if (mapa_[i][j] == 1) {
        obstáculos.push_back(Coordenada{i, j});
      }
    }
  }
  std::shuffle(obstáculos.begin(), obstáculos.end(), rng_);
  for (int k{0}; k < a_liberar && k < static_cast<int>(obstáculos.size()); ++k) {
    mapa_[obstáculos[k].fila][obstáculos[k].columna] = 0; // Liberamos el obstáculo
  }
}
std::ostream& operator<<(std::ostream& os, const Laberinto& laberinto) {
  os << "Número de filas: " << laberinto.GetFilas() << std::endl;
  os << "Número de columnas: " << laberinto.GetColumnas() << std::endl;
  os << "Entrada en: [" << laberinto.ObtenerInicio().fila << ", " << laberinto.ObtenerInicio().columna << "]" << std::endl;
  os << "Salida en: [" << laberinto.ObtenerFin().fila << ", " << laberinto.ObtenerFin().columna << "]" << std::endl;
  os << "Mapa del laberinto:" << std::endl << std::endl;
  for (int i{0}; i < laberinto.GetFilas(); ++i) {
    for (int j{0}; j < laberinto.GetColumnas(); ++j) {
      switch (laberinto.GetMapa()[i][j]) {
        case 1: // son muros, los pinto de blanco (por defecto)
          os << laberinto.GetMapa()[i][j] << " ";
          break;
        case 8:
          os << laberinto.GetMapa()[i][j] << " ";
          break;
        case 3: // pinto de rojo sobre azul claro la entrada y salida
        case 4:
          os << laberinto.GetMapa()[i][j] << " ";
          break;
        default:  // pintamos lo que está libre en color gris (destaca menos)
          os << laberinto.GetMapa()[i][j] << " ";
          break;
      }
    }
    os << std::endl;
  }
  os << std::endl;
  return os;
}