/**
 * @file main_maze.cc
 * @brief Fichero principal para la práctica 2 de IA. 3 año, Ingeniería Informática, Universidad de La Laguna
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#include <iostream>
#include <fstream>
#include <string>

#include "laberinto.h"
#include "AStar.h"
#include "auxiliares.h"

/**
 * @brief Función que muestra la ayuda del programa
 * @param nombre_programa Nombre del programa (argv[0])
 * @return void
 */
void MostrarAyuda(const std::string& nombre_programa) {
  std::cout << "Uso: " << nombre_programa << " <fichero_entrada> <fichero_salida>\n\n"
            << "Ejemplo: " << nombre_programa << " laberinto.txt salida.txt\n\n"
            << "Opciones:\n"
            << "  -h, --help      Muestra esta ayuda y termina.\n";
}


/**
 * @brief Función principal del programa. Carga un laberinto desde un fichero, permite modificar la entrada y salida
 * del laberinto y realiza un recorrido dinámico con A* en el laberinto, imprimiendo el resultado en un fichero de salida.
 * @param argc Número de argumentos
 * @param argv Vector de argumentos
 * @return int Código de salida del programa 
 */
int main(int argc, char* argv[]) {
  system("clear");
  // Comprobamos los argumentos pasados al programa
  if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
    MostrarAyuda(argv[0]);
    return 0;
  }
  // Si no son 3 argumentos (programa, fichero_entrada, fichero_salida) mostramos error y ayuda
  if (argc != 3) {
    std::cerr << "Error: número incorrecto de argumentos.\n";
    MostrarAyuda(argv[0]);
    return 1;
  }
  const std::string fichero_entrada = argv[1];    // nombre del fichero de entrada
  const std::string fichero_salida = argv[2];     // nombre del fichero de salida
  Laberinto laberinto{fichero_entrada};           // creamos el laberinto con el fichero de entrada
  std::ofstream flujo_salida{fichero_salida};     // abrimos el fichero de salida
  if (!flujo_salida) {                             // comprobamos que se ha abierto correctamente
    std::cerr << "Error: no se pudo abrir el fichero de salida " << fichero_salida
              << " para escritura." << std::endl;
    return 1;
  }
  std::cout << "Introduzca un 1 para modificar las casillas de entrada y salida del laberinto, cualquier otra letra para continuar: ";
  char opcion;
  std::cin >> opcion;
  if (opcion == '1') {
    ModificarEntradaSalida(laberinto, flujo_salida);
  }
  // mostramos el laberinto cargado antes de hacer ningún cambio, y mostramos información del mismo
  flujo_salida << "Información del laberinto: " << std::endl << laberinto << "Solución:" << std::endl;
  // creamos el objeto AStar con el laberinto cargado
  AStar recorrido_a_estrella{laberinto};
  // realizamos el recorrido dinámico con A* y comprobamos si ha sido exitoso
  if (!RecorridoDinamico(recorrido_a_estrella, laberinto, flujo_salida)) {
    flujo_salida << "No se ha encontrado un camino desde la entrada hasta la salida." << std::endl;
  }
  return 0;
}