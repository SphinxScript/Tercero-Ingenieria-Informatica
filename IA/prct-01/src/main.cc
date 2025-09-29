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
  std::streambuf* cout_buf = std::cout.rdbuf();   // guardamos el buffer original de cout
  std::ofstream fichero_salida{"salida.txt"};
  std::cout.rdbuf(fichero_salida.rdbuf());         // redirigimos cout al fichero
  if (opcion == 1) {
    std::cout << "Recorrido en profundidad" << std::endl;
    objeto_grafo.RecorridoProfundidad(nodo_partida, nodo_final);
  }
  else if (opcion == 2) {
    std::cout << "Recorrido en amplitud" << std::endl;
    objeto_grafo.RecorridoAmplitud(nodo_partida, nodo_final);
  }
  else {
    std::cerr << "Opción no válida" << std::endl;
    return 1;
  }
  std::cout.rdbuf(cout_buf);                       // restauramos el buffer original de cout
  fichero_salida.close();                          // cerramos el fichero
  std::cout << "Ejecución finalizada. Resultados en 'salida.txt'" << std::endl;
  return 0;
}