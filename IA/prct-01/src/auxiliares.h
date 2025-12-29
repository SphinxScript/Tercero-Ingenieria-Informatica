/**
 * @file auxiliares.h
 * @brief Fichero de cabecera de las funciones auxiliares
 * @author Ricardo David Rodríguez Pane
 * @date 25-12-2025
 * 
 */

#ifndef AUXILIARES_H
#define AUXILIARES_H

#include "grafo.h"
void ShowHelp();
bool CheckArgs(const int argc, char* const argv[]);
std::pair<int, int> HandleNode(int);
void PrintDfs(const std::vector<Nodo>&, int, std::ostream&);
void PrintSummary(std::ostream&, const Grafo&, std::pair<int, int>);
#endif