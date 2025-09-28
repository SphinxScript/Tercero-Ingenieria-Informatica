#include "grafo.h"


int main(int argc, char* argv[]) {
  system("clear");
  std::string nombrefichero{argv[1]};
  if (argc != 2) {
    std::cerr << "Error: se han pasado un número de argumentos no válido" << std::endl;
    return 1;
  }
  int codigo_error;
  Grafo objeto_grafo(nombrefichero, codigo_error);
  if (codigo_error == 1) {
    return 1;
  }
  //std::cout << objeto_grafo;
  std::cout << "Introduzca nodo de partida, y luego nodo final" << std::endl;
  unsigned nodo_partida, nodo_final;
  std::cin >> nodo_partida >> nodo_final;
  system("clear");
  objeto_grafo.RecorridoProfundidad(nodo_partida, nodo_final);
  return 0;
}