#pragma once
#ifndef RIEMANN_H
#define RIEMANN_H

#include <stdlib.h>

#include "func.h"

// Tipos de soma de Riemann (posição dos pontos amostrados).
typedef enum { DIREITA, ESQUERDA } SumType;

// Método público que computa uma soma de Riemann qualquer de uma função
// arbitrária.
double riemann(double min, double max, Function *func, size_t num,
	       SumType type);

#endif // !RIEMANN_H