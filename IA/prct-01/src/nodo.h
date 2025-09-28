#ifndef NODO_H
#define NODO_H

class Nodo {    // Clase para poder reconstruir el camino
 public:
  Nodo* parent; // puntero que apunta al nodo padre; usado para calcular el coste hasta este nodo
  double cost;
  unsigned edge;
  Nodo(unsigned state, Nodo* parent = nullptr, double cost = 0) {
    this->edge = state;
    this->parent = parent;
    if (parent != nullptr) {
      this->cost += parent->cost + cost;
    }
    else {
      this->cost = cost;
    }
  }
};

#endif