/**
 * @file auxiliares.h
 * @brief Fichero con las declaraciones de las funciones auxiliares para la práctica 2 de IA. 3 año, Ingeniería Informática, Universidad de La Laguna
 * @author Ricardo David Rodríguez Pane
 * @date Octubre 2025
 */

#ifndef AUX_H
#define AUX_H

#include "AStar.h"
#include "laberinto.h"

/**
 * @brief Función que imprime el laberinto  con solución en un stream de salida
 * @param recorrido Objeto AStar con el recorrido planificado
 * @param laberinto Objeto Laberinto con el mapa
 * @param os Stream de salida
 * @param recorrido_dinamico Vector de Coordenada con el recorrido real realizado hasta el momento
 * @return void
 * @note Si recorrido_dinamico está vacío, no se imprime el recorrido dinámico
 */
void ImprimeCamino(const AStar& recorrido, const Laberinto& laberinto, std::ostream& os, const std::vector<Coordenada>& recorrido_dinamico = {});

/**
 * @brief Función que modifica las coordenadas de entrada y salida del laberinto
 * @param laberinto Objeto Laberinto a modificar
 * @param os Stream de salida que se encarga de imprimir las instrucciones para el usuario
 * @return void
 */
bool ModificarEntradaSalida(Laberinto& laberinto, std::ostream& os);


/**
 * @brief Función principal de la práctica. Realiza un recorrido dinámico con A* en un laberinto donde los obstáculos
 * se mueven en cada iteración. Nótese por iteración cada "paso" que da el agente en el laberinto. En cada paso, se realiza A* con
 * los obstáculos recalculados. El % de obstáculos en el laberinto ha de ser < 25% para asegurar la existencia de caminos. Si a pesar de
 * ello no se encuentra camino, se reintenta hasta 5 veces, recalculando el A* en esa posición hasta el momento. Si no se encuentra
 * camino en 5 intentos, se da por inalcanzable la meta.
 * @param recorrido Objeto AStar para realizar el recorrido
 * @param laberinto Objeto Laberinto donde se realiza el recorrido
 * @param os Stream de salida donde se imprime el laberinto y el camino en cada paso
 */
bool RecorridoDinamico(AStar& recorrido, Laberinto& laberinto, std::ostream& os);

/**
 * @brief Función auxiliar que calcula el coste del paso entre dos coordenadas adyacentes. No usamos el método de la clase porque es privado.
 * @param actual Coordenada actual
 * @param siguiente Coordenada siguiente
 * @return int Coste del paso
 * @note El coste es 5 para movimientos ortogonales y 7 para movimientos diagonales
 */
inline int PasoCoste(const Coordenada& actual, const Coordenada& siguiente) {
  const bool orto = (actual.fila == siguiente.fila) ^ (actual.columna == siguiente.columna);
  const bool diag = (actual.fila != siguiente.fila) && (actual.columna != siguiente.columna);
  return orto ? 5 : (diag ? 7 : 0);
}

/**
 * @brief Función que permite modificar las probabilidades de aparición y desaparición de obstáculos en el laberinto
 * @param laberinto Objeto Laberinto donde se modificarán las probabilidades
 * @return true si se han modificado correctamente, false en caso contrario
 */
bool ModificaProbabilidades(Laberinto& laberinto);

#endif