#ifndef GRAFO_H
#define GRAFO_H


// este struct se utilizará para almacenar la información de un nodo. Un elemento de la lista de adyacencia, que es lo que usaré para saber sucesores, etc.
// Posteriormente usaré esta lista y haré un vector de 
struct ElementolistaAdyacencia {
  unsigned nodo;
  int coste;
  ElementolistaAdyacencia(unsigned nodo_param, int coste_param = 1)
  : nodo{nodo_param}, coste{coste_param} {}
};

// struct que se utilizará para almacenar la información de una arista. ambos extremos y coste.
struct Arista {
  unsigned extremo1;
  unsigned extremo2;
  int coste;
  Arista(unsigned arista1, unsigned arista2)
  : extremo1{arista1}, extremo2{arista2} {}
};

#endif