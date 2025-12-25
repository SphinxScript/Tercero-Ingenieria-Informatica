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

int main(int argc, char* argv[]) {
  system("clear");
  // comprobamos argumentos pasados al programa
  for (int i{1}; i < argc; ++i) {
    if (std::string(argv[i]) == "--h" || std::string(argv[i]) == "--help") {
      ShowHelp();
      return 2;
    }
    else continue;
  }
  // si se pasan más de 3 argumentos ignoramos el resto
  if (argc < 3) {
    ShowHelp();
    return 2;
  }
  return 0;
}