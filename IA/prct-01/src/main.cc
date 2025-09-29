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
  std::cout << objeto_grafo;
//  objeto_grafo.ImprimeResumen();
  std::cout << "Introduzca opción de operación: 1 para recorrer el grafo por DFS, 2 para recorrer por BFS" << std::endl;
  int opcion;
  std::cin >> opcion;
  std::cout << "Introduzca nodo de partida, y luego nodo final" << std::endl;
  unsigned nodo_partida, nodo_final;
  std::cin >> nodo_partida >> nodo_final;
  system("clear");
  if (opcion == 1) {
    objeto_grafo.RecorridoProfundidad(nodo_partida, nodo_final);
  }
  else if (opcion == 2) {
  //  objeto_grafo.RecorridoBreadthFirst(nodo_partida, nodo_final);
  }
  else {
    std::cerr << "Opción no válida" << std::endl;
    return 1;
  }
  return 0;
}