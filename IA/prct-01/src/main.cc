#include "grafo.h"


int main(int argc, char* argv[]) {
  std::string nombrefichero{argv[1]};
  if (argc != 2) {
    std::cerr << "Error: se han pasado un número de argumentos no válido" << std::endl;
    return 1;
  }
  int codigo_error;
  Grafo objeto_grafo(nombrefichero, codigo_error);
  std::cout << objeto_grafo;
  return 0;
}