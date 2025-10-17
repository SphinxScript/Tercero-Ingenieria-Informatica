/**
 * @file laberinto.cc
 * @brief Implementación de la clase Laberinto para la práctica 2 de IA.
 * 3 año, Ingeniería Informática, Universidad de La Laguna
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <random>     // para la generación del número aleatorio
#include <algorithm>  // para std::shuffle
#include <chrono>     // para la semilla del generador de números aleatorios
#include "laberinto.h"


/**
 * @brief Constructor de la clase Laberinto que inicializa el laberinto desde un fichero
 * @param nombre_fichero Nombre del fichero desde el que se carga el laberinto
 */
Laberinto::Laberinto(const std::string& nombre_fichero) {
  // Inicializamos el nombre del fichero
  nombre_fichero_ = nombre_fichero;
  // Inicializamos el generador de números aleatorios con una semilla basada en el
  // tiempo y la dirección del objeto
  const auto t = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::seed_seq seq{ static_cast<uint32_t>(t), static_cast<uint32_t>(t >> 32), static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this))};
  // inicializamos el motor de generación de números aleatorios
  rng_.seed(seq);


  bool cargado = Cargar();
  if (!cargado) {
    std::cerr << "Error al cargar el laberinto desde el fichero" << std::endl;
    filas_ = 0;
    columnas_ = 0;
  }

  // Inicializamos el motor de generación de números aleatorios
  std::mt19937 rng(std::random_device{}());
}

/**
 * @brief Carga el laberinto desde el fichero especificado en el constructor
 * @return true si se ha cargado correctamente, false en caso contrario
 */
bool Laberinto::Cargar() {
  std::ifstream fichero{nombre_fichero_};
  if (!fichero.is_open()) {
    std::cerr << "Error al abrir el fichero" << std::endl;
    return false;
  }
  // Leemos las dimensiones del laberinto
  fichero >> filas_ >> columnas_;
  // Redimensionamos el mapa del laberinto
  mapa_.resize(filas_, std::vector<int>(columnas_));
  // Leemos el mapa del laberinto desde el fichero
  // localizamos entrada y salida mientras leemos el mapa
  // y los inicializamos en inicio_ y fin_
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

/**
 * @brief Comprueba si una celda del laberinto es transitable
 * @param fila Fila de la celda a comprobar
 */
bool Laberinto::EsTransitable(int fila, int columna) const {
  return mapa_[fila][columna] != 1 ? true : false;
}

/**
 * @brief Modifica la posición de la entrada o salida del laberinto
 * @param fila Fila de la nueva posición
 * @param columna Columna de la nueva posición
 * @param selector Selector para indicar si se modifica la entrada (0) o la salida (
 * @return true si la operación se ha realizado correctamente, false en caso contrario
 */
bool Laberinto::ModificarEntradaSalida(int fila, int columna, int selector) {
  bool operacion = true;
  // comprobamos que las coordenadas están dentro del laberinto
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
    else {    // si vamos por aqui, es que modificamos la salida
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

/**
 * @brief Actualiza los obstáculos del laberinto según las probabilidades dadas
 * @param pin Probabilidad de que una celda libre se convierta en obstáculo
 * @param pout Probabilidad de que un obstáculo se convierta en celda libre
 */
void Laberinto::ActualizaObstaculos(double pin, double pout) {
  // recorremos todo el mapa del laberinto para actualizar los obstáculos
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      // si es la entrada o la salida, no la modificamos
      if (mapa_[i][j] == 3 || mapa_[i][j] == 4) {
        continue;
      }
      // si es libre, puede convertirse en obstáculo con probabilidad pin
      if (mapa_[i][j] == 0) {
        double probabilidad = uni01_(rng_); // generamos un número aleatorio entre 0 y 1
        if (probabilidad < pin) {   // si el número es menor que pin, lo convertimos en obstáculo
          mapa_[i][j] = 1;
        }
      }
      // si es un obstáculo, puede convertirse en libre con probabilidad pout
      else if (mapa_[i][j] == 1) {
        double probabilidad = uni01_(rng_); // generamos un número aleatorio entre 0 y 1
        if (probabilidad < pout) {          // si el número es menor que pout, lo convertimos en libre
          mapa_[i][j] = 0;
        }
      }
      else {
        continue;   // si es cualquier otro valor, no lo modificamos
      }
    }
  }
  // comprobamos si el porcentaje de obstáculos es menor del 25%
  int max_obstaculos = (filas_ * columnas_) * 0.25;   // 25% del total de casillas. Entero porque la diferencia es pequeña
  if (NumeroObstaculos() >= max_obstaculos) {    // En caso de que haya más del 25%, llamamos a EliminaObstaculosAzar
    EliminaObstaculosAzar(NumeroObstaculos() - max_obstaculos);   // eliminamos el número de obstaculos de la diferencia con el 25%
  }
}

/**
 * @brief Cuenta el número de obstáculos en el laberinto. Se emplea para saber el % de obstáculos
 * @return Número de obstáculos en el laberinto
 */
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

/**
 * @brief Elimina un número específico de obstáculos al azar en el laberinto. Número conocido porque
 * se ha calculado previamente con el número de obstáculos que hay y el máximo permitido
 * teniendo en cuenta el porcentaje querido.
 * @param a_liberar Número de obstáculos a eliminar
 */
void Laberinto::EliminaObstaculosAzar(int a_liberar) {
  // Creamos un vector que almacena las coordenadas de los obstáculos
  std::vector<Coordenada> obstáculos;
  for (int i{0}; i < filas_; ++i) {
    for (int j{0}; j < columnas_; ++j) {
      if (mapa_[i][j] == 1) {
        obstáculos.push_back(Coordenada{i, j});
      }
    }
  }
  // Barajamos el vector de obstáculos
  std::shuffle(obstáculos.begin(), obstáculos.end(), rng_);
  // Liberamos los primeros a_liberar obstáculos
  for (int k{0}; k < a_liberar && k < static_cast<int>(obstáculos.size()); ++k) {
    mapa_[obstáculos[k].fila][obstáculos[k].columna] = 0; // Liberamos el obstáculo
  }
}

/**
 * @brief Sobrecarga del operador de salida para imprimir el laberinto
 * @param os Flujo de salida
 * @param laberinto Laberinto a imprimir
 * @return Flujo de salida
 */
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