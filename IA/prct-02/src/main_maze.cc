#include <iostream>
#include <fstream>
#include <string>

#include "laberinto.h"


void MostrarAyuda(const std::string& nombre_programa) {
  std::cout << "Uso: " << nombre_programa << " <fichero_entrada> <fichero_salida>\n\n"
            << "Ejemplo: " << nombre_programa << " laberinto.txt salida.txt\n\n"
            << "Opciones:\n"
            << "  -h, --help      Muestra esta ayuda y termina.\n";
}


int main(int argc, char* argv[]) {
  if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
    MostrarAyuda(argv[0]);
    return 0;
  }
  if (argc != 3) {
    std::cerr << "Error: número incorrecto de argumentos.\n";
    MostrarAyuda(argv[0]);
    return 1;
  }
  const std::string fichero_entrada = argv[1];
  const std::string fichero_salida = argv[2];
  Laberinto laberinto(fichero_entrada);
  std::ofstream ofs{fichero_salida};
  ofs << laberinto;

  return 0;
}


