#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>
#include <fstream>

/*
  este struct se utilizará para almacenar la información de un nodo. Un elemento de la lista de adyacencia, que es lo que usaré para saber sucesores, etc.
  se utilizará este struct para almacenarlo posteriormente en un vector, donde dicho vector almacena un conjunto de ElementoAdyacencia
  posteriormente otro vector almacena ese vector ElementoAdyacencia> (vector<vector<ElementoAdyacencia>>) y este vector se usará para
  emplear como índice un nodo, y pues dicho nodo tiene un conjunto de adyacentes, con su coste (el de ElementoAdyacencia).
  representa el vecino de un nodo con su coste
*/
struct ElementoAdyacencia {
  unsigned nodo;
  double coste;
  ElementoAdyacencia() = default;
  ElementoAdyacencia(unsigned nodo_param, int coste_param = 1)
  : nodo{nodo_param}, coste{coste_param} {}
};

// struct que se utilizará para almacenar la información de una arista. ambos extremos y coste.
struct Arista {
  unsigned extremo1;
  unsigned extremo2;
  int coste;
  Arista() = default;
  Arista(unsigned arista1, unsigned arista2)
  : extremo1{arista1}, extremo2{arista2} {}
};

typedef std::vector<ElementoAdyacencia> nodo_lista;

class Grafo {
 public:
  Grafo() = default;
  Grafo(const std::string& nombre_fichero, int& error_apertura);    // Aquí inicializaré la lista de adyacencia del grafo. Llamará a build
  ~Grafo() = default;

 private:
  std::vector<nodo_lista> ListaAdyacencia_;
  unsigned nodos; // número de nodos
  unsigned arcos; // número de arcos

  void Build(const std::string& fichero_string, int& error);  // devuelve 0 en caso de éxito, 1 en caso contrario

};

#endif