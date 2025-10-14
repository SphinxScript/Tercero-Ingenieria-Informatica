#ifndef LABERINTO_H
#define LABERINTO_H

#include <vector>
#include <string>

struct Coordenada {
  int fila;
  int columna;
};

class Laberinto {
 public:
  Laberinto() = default;
  Laberinto(const std::string& nombre_fichero);
  bool Cargar();
  void Mostrar() const;
  void ActualizaObstáculos();
  void ModificarEntradaSalida();
  bool EsTransitable(int fila, int columna) const;
  Coordenada ObtenerInicio() const { return inicio_; }
  Coordenada ObtenerFin() const { return fin_; }
  int GetFilas() const { return filas_; }
  int GetColumnas() const { return columnas_; }
  const std::vector<std::vector<int>>& GetMapa() const { return mapa_; }
 private:
  void ModificarEntradaSalida_private(const Coordenada& coordenada);
  std::string nombre_fichero_;
  std::vector<std::vector<int>> mapa_;
  Coordenada inicio_;
  Coordenada fin_;
  int filas_;
  int columnas_;
};

std::ostream& operator<<(std::ostream& os, const Laberinto& laberinto);

#endif