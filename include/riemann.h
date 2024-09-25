/**
 * @file riemann.h
 * @brief Declarações de tipos e funções para cálculo de somas de Riemann.
 *
 * @details Este arquivo contém as declarações de tipos e funções utilizadas
 * para calcular somas de Riemann de funções arbitrárias. Ele define o tipo de
 * soma de Riemann (esquerda ou direita) e a função que realiza o cálculo da
 * soma de Riemann em um intervalo especificado.
 */

#pragma once
#ifndef RIEMANN_H
#define RIEMANN_H

#include <stdlib.h>
#include "func.h"

/**
 * @brief Tipos de soma de Riemann.
 * @details Corresponde a qual das arestas dos retângulos (direita ou esquerda)
 * os valores de \f$x_i\f$ serão amostrados.
 */
typedef enum {
	DIREITA, /**< Soma de Riemann pela direita. */
	ESQUERDA /**< Soma de Riemann pela esquerda. */
} SumType;

/**
 * @brief Calcula uma soma de Riemann de uma função arbitrária em um intervalo.
 *
 * @details Esta função calcula a soma de Riemann de uma função arbitrária no
 * intervalo \f$[min, max]\f$ usando um número especificado de retângulos. O
 * tipo de soma de Riemann (esquerda ou direita) é determinado pelo parâmetro
 * `type`.
 *
 * @param min O limite inferior do intervalo de integração.
 * @param max O limite superior do intervalo de integração.
 * @param func Ponteiro para a função a ser integrada.
 * @param num O número de retângulos a serem usados na soma de Riemann.
 * @param type O tipo de soma de Riemann (esquerda ou direita).
 * @return O valor aproximado da integral da função no intervalo especificado.
 */
double riemann(double min, double max, Function *func, size_t num,
	       SumType type);

#endif // !RIEMANN_H