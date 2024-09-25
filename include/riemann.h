#pragma once
#ifndef RIEMANN_H
#define RIEMANN_H

#include <stdlib.h>

#include "func.h"

// Tipos de soma de Riemann (posição dos pontos amostrados).
typedef enum { DIREITA, ESQUERDA } SumType;

double riemann(double a, double b, Function *func, size_t n, SumType t);

#endif // !RIEMANN_H