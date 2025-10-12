#include <fstream>
#include <sstream>
#include <iostream>
#include "laberinto.h"


Laberinto::Laberinto(const std::string& nombre_fichero) {
  nombre_fichero_ = nombre_fichero;

  bool cargado = Cargar();
  if (!cargado) {
    std::cerr << "Error al cargar el laberinto desde el fichero" << std::endl;
    filas_ = 0;
    columnas_ = 0;
  }

  
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
      if (mapa_[i][j] == 2) {
        inicio_.fila = i;
        inicio_.columna = j;
      }
      else if (mapa_[i][j] == 3) {
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

std::ostream& operator<<(std::ostream& os, const Laberinto& laberinto) {
  os << "Número de filas: " << laberinto.GetFilas() << std::endl;
  os << "Número de columnas: " << laberinto.GetColumnas() << std::endl;
  os << "Mapa del laberinto:" << std::endl;
  for (int i{0}; i < laberinto.GetFilas(); ++i) {
    for (int j{0}; j < laberinto.GetColumnas(); ++j) {
      os << laberinto.GetMapa()[i][j] << " ";
    }
    os << std::endl;
  }
  return os;
}