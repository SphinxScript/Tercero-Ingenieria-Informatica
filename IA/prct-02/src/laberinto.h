/**
 * @file laberinto.h
 * @brief Fichero de cabecera para la clase Laberinto de la práctica
 * 2 de IA. 3 año, Ingeniería Informática, Universidad de La Laguna
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#ifndef LABERINTO_H
#define LABERINTO_H

#include <vector>
#include <string>
#include <random>

/**
 * @brief Estructura que representa una coordenada en el laberinto
 */
struct Coordenada {
  int fila;
  int columna;
};

/**
 * @class Laberinto
 * @brief Clase que representa un laberinto y sus operaciones asociadas
 */
class Laberinto {
 public:
  Laberinto() = default;
  Laberinto(const std::string& nombre_fichero);
  bool Cargar();
  void ActualizaObstáculos();
  bool ModificarEntradaSalida(int fila, int columna, int selector);   // selector: 0 para entrada, 1 para salida
  bool EsTransitable(int fila, int columna) const;
  Coordenada ObtenerInicio() const { return inicio_; }
  Coordenada ObtenerFin() const { return fin_; }
  int GetFilas() const { return filas_; }
  int GetColumnas() const { return columnas_; }
  void ActualizaObstaculos(double pin = 0.5, double pout = 0.5);
  const std::vector<std::vector<int>>& GetMapa() const { return mapa_; }

 private:
  int NumeroObstaculos() const;

  // opcode 0 -> elimina el número especificado, 1 -> elimina obstáculos al azar (por si no hay salida)
  void EliminaObstaculosAzar(int a_liberar);    
  std::string nombre_fichero_;
  std::vector<std::vector<int>> mapa_;
  Coordenada inicio_;
  Coordenada fin_;
  int filas_;
  int columnas_;
  // declaramos el motor para los números aleatorios para la actualización de obstáculos
  std::mt19937 rng_;
  std::uniform_real_distribution<double> uni01_{0.0, 1.0};
};

/**
 * @brief Sobrecarga del operador de salida para imprimir el laberinto
 * @param os Flujo de salida
 * @param laberinto Laberinto a imprimir
 * @return Flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Laberinto& laberinto);

#endif