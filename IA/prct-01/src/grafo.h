#ifndef GRAFO_H
#define GRAFO_H


/*
  este struct se utilizará para almacenar la información de un nodo. Un elemento de la lista de adyacencia, que es lo que usaré para saber sucesores, etc.
  se utilizará este struct para almacenarlo posteriormente en un vector, donde dicho vector almacena un conjunto de ElementoAdyacencia
  posteriormente otro vector almacena ese vector ElementoAdyacencia> (vector<vector<ElementoAdyacencia>>) y este vector se usará para
  emplear como índice un nodo, y pues dicho nodo tiene un conjunto de adyacentes, con su coste (el de ElementoAdyacencia).
*/
struct ElementoAdyacencia {
  unsigned nodo;
  int coste;
 ElementoAdyacencia(unsigned nodo_param, int coste_param = 1)
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

class Grafo {
  public:

  private:


};

#endif