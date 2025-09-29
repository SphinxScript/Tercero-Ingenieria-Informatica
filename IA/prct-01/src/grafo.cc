#include <stack>
#include <algorithm>
#include <queue>

#include "grafo.h"


// Definimos el constructor
Grafo::Grafo(const std::string& nombre_fichero, int& error_apertura) {
  Build(nombre_fichero, error_apertura);
}

// Método que construye el grafo a partir de un fichero
void Grafo::Build(const std::string& fichero_string, int& error) {
  std::ifstream fichero{fichero_string};
  if (!fichero.is_open()) {
    std::cout << "Error al abrir el fichero" << std::endl;
    error = 1;
    return;
  }
  double costo;
  fichero >> nodos_;
  lista_adyacencia_.resize(nodos_);
  int contador_aristas{0};
  for (int i{0}; i < nodos_ - 1; ++i) {
    for (int j{i + 1}; j < nodos_; ++j) {
      fichero >> costo;
      if (costo != -1) {
        ++contador_aristas;
        lista_adyacencia_[i].push_back(ElementoAdyacencia(j, costo));
        lista_adyacencia_[j].push_back(ElementoAdyacencia(i, costo));
      }
    }
  }
  arcos_ = contador_aristas;
  fichero.close();
}

// Método que imprime un resumen del grafo
void Grafo::ImprimeResumen() const {
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "Número de nodos del grafo: " << nodos_ << std::endl; 
  std::cout << "Número de aristas del grafo: " << arcos_ << std::endl;
}

bool Grafo::CompruebaDestino(unsigned& nodo_destino, unsigned& nodo_actual) const {
  return (nodo_actual == nodo_destino) ? true : false;
}


// Método de recorrido en profundidad
void Grafo::RecorridoProfundidad(unsigned& nodo_inicio, unsigned& nodo_final) const {
  Grafo grafo = *this;
  grafo.ImprimeResumen();
  std::cout << "Vértice origen: " << nodo_inicio << std::endl;
  std::cout << "Vértice destino: " << nodo_final << std::endl;
  std::vector<unsigned> padres;
  --nodo_inicio;
  --nodo_final;
  Dfs(nodo_inicio, nodo_final, padres);
  std::vector<unsigned> camino;  // vector de nodos del camino
  unsigned nodo_camino = nodo_final;
  while (true) {                        // bucle while para recrear el camino con el vector padres
    camino.push_back(nodo_camino);      // se agrega el nodo al vector camino
    if (nodo_camino == nodo_inicio) break; // si se ha llegado al nodo inicial, se sale del bucle
    nodo_camino = padres[nodo_camino];     // ahora seguimos el bucle con el nodo padre del actual
  }   // El bucle va desde el nodo final hasta el nodo inicial, recorriendo el vector padres. Cuando el nodo del camino es el nodo_inicial es que hemos llegao.
  std::reverse(camino.begin(), camino.end());   // se invertimos el vector camino, ya que va desde el nodo final al nodo iniical.
  std::cout << "Camino: ";                      // imprimimos el camino
  PrintVector(camino);
  double coste = CalculaCoste(camino, nodo_inicio);   // Llamamos a la funcion calculacoste que calcula el coste de un vector de nodos hasta un nodo inicial.
  std::cout << "Coste: " << coste << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
}

void Grafo::Dfs(const unsigned& nodo_inicial, const unsigned& nodo_final, std::vector<unsigned>& padres) const {
  std::stack<unsigned> pila_nodos;  // pila de nodos a explorar
  pila_nodos.push(nodo_inicial);    // nodo inicial
  std::vector<bool> visitados;      // vector de bool para marcar los nodos visitados
  visitados.resize(nodos_, false);  // inicializamos el vector de bool
  bool encontrado = false;          // variable para saber si se ha encontrado el nodo final
  int contador{0};                  // contador de iteraciones
  padres.resize(nodos_);            // vector de padres
  std::vector<unsigned> generados;  // vector de nodos generados
  std::vector<unsigned> inspeccionados;  // vector de nodos inspeccionados
  unsigned nodo_actual{__UINT8_MAX__};             // nodo actual
  generados.push_back(nodo_inicial);
  while (!pila_nodos.empty() && !encontrado) {    // bucle while para recorrer la pila de nodos
    if (nodo_actual == nodo_final) {               // si se ha encontrado el nodo final
      encontrado = true;                           // se marca que se ha encontrado el nodo final y se imprime generados e inspeccionados y se sigue con el bucle
      std::cout << "---------------------------------------------" << std::endl; 
      std::cout << "Iteración: " << ++contador << std::endl;  
      std::cout << "Nodos generados: ";
      PrintVector(generados);
      std::cout << "Nodos inspeccionados: ";
      PrintVector(inspeccionados);
      continue;
    }
    std::cout << "---------------------------------------------" << std::endl;  // Imprimimos las iteraciones con vector de generados e inspeccionados
    std::cout << "Iteración: " << ++contador << std::endl;
    std::cout << "Nodos generados: ";
    PrintVector(generados);
    std::cout << "Nodos inspeccionados: ";
    PrintVector(inspeccionados);
    nodo_actual = pila_nodos.top();  // se saca el primer nodo de la pila
    pila_nodos.pop();                // se saca el primer nodo de la pila
    visitados[nodo_actual] = true;   // se marca el nodo como visitado en el vector de bool visitados
    inspeccionados.push_back(nodo_actual);  // se agrega el nodo a inspeccionados (es el nodo a inspeccionar)
    for (int i{lista_adyacencia_[nodo_actual].size() - 1}; i >= 0; --i) {   // Bucle for para insertar en la pila los sucesores del nodo actual en orden inverso
      unsigned siguiente = lista_adyacencia_[nodo_actual][i].nodo;    // extraemos el siguiente nodo al actual
      if (!visitados[siguiente]) {                                   // si el siguiente nodo no ha sido visitado
        pila_nodos.push(lista_adyacencia_[nodo_actual][i].nodo);     // se agrega el siguiente nodo a la pila
        padres[siguiente] = nodo_actual;                             // marcamos en el vector de padres que padre del siguiente es el actual
      }
    }
    for (int i{0}; i < lista_adyacencia_[nodo_actual].size(); ++i) {   // Este bucle recorre los sucesores del nodo actual en orden creciente, para ponerlos en el vector generados
      unsigned siguiente = lista_adyacencia_[nodo_actual][i].nodo;    // extraemos el siguiente nodo al actual
      if (!visitados[siguiente]) {                                   // si el siguiente nodo no ha sido visitado
        generados.push_back(lista_adyacencia_[nodo_actual][i].nodo);  // se agrega el siguiente nodo a la pila
      }
    }
  }
  std::cout << "---------------------------------------------" << std::endl;
}


void Grafo::RecorridoAmplitud(unsigned& nodo_inicio, unsigned& nodo_final) const {
  Grafo grafo = *this;
  grafo.ImprimeResumen();
  std::cout << "Vértice origen: " << nodo_inicio << std::endl;
  std::cout << "Vértice destino: " << nodo_final << std::endl;
  --nodo_inicio;
  --nodo_final;
  std::vector<unsigned> padres;
  Bfs(nodo_inicio, nodo_final, padres);
  std::vector<unsigned> camino;   // Vector de nodos del camino
  unsigned nodo_camino = nodo_final;
  while (true) {
    camino.push_back(nodo_camino);  // metemos en el camino el nodo del camino en cada momento
    if (nodo_camino == nodo_inicio) break;  // en caso de que el nodo actual sea el inicial, hemos recorrido el camino de padres hasta el inicio
    nodo_camino = padres[nodo_camino];
  }
  std::reverse(camino.begin(), camino.end());
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "Camino: ";
  PrintVector(camino);
  double coste{CalculaCoste(camino, nodo_inicio)};
  std::cout << "Coste: " << coste << std::endl;
}

void Grafo::Bfs(const unsigned& nodo_inicial, const unsigned& nodo_final, std::vector<unsigned>& padres) const {
  std::queue<unsigned> cola_nodos;
  cola_nodos.push(nodo_inicial);
  std::vector<bool> visitados(nodos_, false);
  visitados[nodo_inicial] = true;  // importante
  padres.resize(nodos_);
  std::vector<unsigned> generados;
  std::vector<unsigned> inspeccionados;
  generados.push_back(nodo_inicial);
  int contador = 0;
  while (!cola_nodos.empty()) {
    unsigned nodo_actual = cola_nodos.front();
    cola_nodos.pop();
    inspeccionados.push_back(nodo_actual);
    // imprimir estado en cada iteración
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Iteración: " << ++contador << std::endl;
    std::cout << "Nodos generados: ";
    PrintVector(generados);
    std::cout << "Nodos inspeccionados: ";
    PrintVector(inspeccionados);
    if (nodo_actual == nodo_final) break;  // si se ha encontrado el nodo final, se sale del bucle
    for (unsigned i = 0; i < lista_adyacencia_[nodo_actual].size(); ++i) {  // recorremos los sucesores del nodo actual
      unsigned siguiente = lista_adyacencia_[nodo_actual][i].nodo;          // extraemos el siguiente nodo
      if (!visitados[siguiente]) {                                 // si no ha sido visitado
        cola_nodos.push(siguiente);       // se agrega a la cola
        visitados[siguiente] = true;       // se marca como visitado el nodo siguiente
        padres[siguiente] = nodo_actual;   // se marca en el vector de padres que el padre del siguiente es el actual
        generados.push_back(siguiente);    // se agrega a generados
      }
    }
  }
}

void Grafo::PrintVector (const std::vector<unsigned>& vector) const {
  if (vector.size() == 0) {
    std::cout << "-";
  }
  else {
    for (const unsigned& elemento : vector) {
      std::cout << elemento + 1 << " ";
    }
  }
  
  std::cout << std::endl;
}

// Método que calcula el coste de un vector de nodos hasta un nodo inicial
double Grafo::CalculaCoste(const std::vector<unsigned>& camino, const unsigned& nodo_inicial) const {
  double coste_total = 0.0;
  for (int i{0}; i < camino.size(); ++i) {    // recorremos el vector de nodos pasado
    unsigned nodo_actual = camino[i];         // extraemos el nodo actual
    unsigned nodo_sig = camino[i + 1];        // extraemos el siguiente nodo
    for (const auto& elemento : lista_adyacencia_[nodo_actual]) {  // recorremos la lista de adyacencia del nodo actual
      if (elemento.nodo == nodo_sig && nodo_sig != nodo_inicial) {  // si el siguiente nodo es el siguiente en nuestro vector (está en el camino)
        coste_total += elemento.coste;        // y es distinto al nodo inicial (Para evitar sumar el coste en caso de ser un bucle) se suma el coste y dejamos de recorrer los sucesores
        break;
      }
    }
  }
  return coste_total;
}



std::ostream& operator<<(std::ostream& os, const Grafo& grafo) {
  std::cout << "Lista de Adyacencia del grafo:" << std::endl;
  std::vector<nodo_lista> lista{grafo.GetLista()};
  for (int i{0}; i < lista.size(); ++i) {
    std::cout << "Adyacentes de " << i + 1;
    if (lista[i].size() == 0) {
      std::cout << ": []";
    }
    else {
      std::cout << ": [";
    }
    for (int j{0}; j < lista[i].size() && lista[i].size() != 0; ++j) {
      if (j == lista[i].size() - 1) {
        std::cout << lista[i][j].nodo + 1 << "]";
        break;
      }
      std::cout << lista[i][j].nodo + 1 << ", ";
    }
    std::cout << std::endl;
  }
  return os;
}