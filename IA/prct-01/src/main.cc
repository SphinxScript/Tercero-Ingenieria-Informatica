/**
 * @file main.cc
 * @brief Fichero principal de la práctica 01 de IA.
 * @author Ricardo David Rodríguez Pane
 * @date 25-12-2025
 * 
 */

#include <iostream>
#include <fstream>
#include <string>

#include "auxiliares.h"
#include "grafo.h"

int main(int argc, char* argv[]) {
  system("clear");
  // comprobamos argumentos pasados al programa
  if (CheckArgs(argc, argv)) {
//    std::cout << "CheckARgs\n";
    ShowHelp();
    return 2;
  }
  // si se pasan más de 3 argumentos ignoramos el resto
  if (argc < 3) {
//    std::cout << "argc < 3\n";
    ShowHelp();
    return 2;
  }
  std::string infile{argv[1]};
  std::string oufile{argv[2]};
  std::ifstream flujo_entrada(infile);
  if (!flujo_entrada.is_open()) {
    std::cout << "No se puede abrir el fichero\n"
              << "Asegúrese de haber escrito correctamente la dirección\n";
    return 1;
  }
  Grafo grafo(flujo_entrada);
  flujo_entrada.close();


  return 0;
}