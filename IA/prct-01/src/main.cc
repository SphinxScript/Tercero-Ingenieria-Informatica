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
    ShowHelp();
    return 2;
  }
  // si se pasan más de 3 argumentos ignoramos el resto
  if (argc < 3) {
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
  std::pair<int, int> inicio_fin = HandleNode(grafo.GetVertices());
  std::vector<Nodo> padres;
  std::ofstream flujo_salida(oufile);
  PrintSummary(flujo_salida, grafo, inicio_fin);
  int opcion;
  while (opcion < 1 || opcion > 2) {
    std::cout << "Seleccione opción de recorrido:\n"
              << "1. Recorrido en Profundidad\n"
              << "2. Recorrido en amplitud\n";
    std::cin >> opcion;
    if (opcion == 1) {
      std::cout << "Realizando recorrido en profundidad..." << std::endl;
      std::cout << "Resultado en: " << oufile << std::endl;
      if (grafo.RecorridoProfundidad(inicio_fin, padres, flujo_salida)) {
        flujo_salida << "Camino Encontrado!!" << std::endl;
        PrintDfs(padres, (inicio_fin.second - 1), flujo_salida);
      }
      else flujo_salida << "Camino no encontrado :(" << std::endl;
    }
    else if (opcion == 2) {
      std::cout << "Realizando recorrido en amplitud..." << std::endl;
      std::cout << "Resultado en: " << oufile << std::endl;
      if (grafo.RecorridoAmplitud(inicio_fin, padres, flujo_salida)) {
        flujo_salida << "Camino Encontrado!!" << std::endl;
        PrintDfs(padres, (inicio_fin.second - 1), flujo_salida);
      }
      else flujo_salida << "Camino no encontrado :(" << std::endl;
    }
    else {
      std::cout << "Opción no válida. Repita." << std::endl;
    }
  }
  return 0;
}