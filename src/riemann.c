// SPDX-License-Identifier: ISC

/**
 * @file riemann.c
 * @brief Implementação das somas de Riemann para integração numérica.
 *
 * @details Este arquivo contém as implementações das funções para calcular a
 * integral de uma função genérica usando somas de Riemann. As somas podem ser
 * calculadas utilizando a extremidade direita ou esquerda dos retângulos. As
 * funções aqui definidas permitem a integração numérica de funções
 * encapsuladas na estrutura `Function`.
 */

#include <stdio.h>

#include "riemann.h"
#include "func.h"

// Declarações internas.
static double riemann_esq(double a, Function *func, size_t n, double dx);
static double riemann_dir(double a, Function *func, size_t n, double dx);

// Método público. Determina os limites de integração [min, max] a partir dos
// parâmetros, e executa a soma de Riemann da função dada pela referência
// `func` nesse intervalo, usando `num` retângulos. O parâmetro `type` indica
// se o cálculo da função será com x na extremidade direita ou esquerda dos
// retângulos.
double riemann(double min, double max, Function *func, size_t num, SumType type)
{
	// Calcula o Δx a partir do intervalo e do número de retângulos.
	double dx = (max - min) / num;

	// Dependendo de se a soma for pela esquerda ou pela direita, invoca a
	// implementação apropriada.
	switch (type) {
	case DIREITA:
		return riemann_dir(min, func, num, dx);
	case ESQUERDA:
		return riemann_esq(min, func, num, dx);
	default: // Se não for direita nem esquerda, termina programa com erro.
		fprintf(stderr, "FATAL: tipo de soma inexistente: %d\n", type);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Calcula a soma de Riemann pela direita.
 *
 * @details Para cada \f$i\f$ inteiro no intervalo \f$[1, n]\f$, esta função
 * avalia a função arbitrária `func` em \f$x_i = a + i \cdot \Delta x\f$, onde
 * \f$a\f$ é o limite inferior da integração, e \f$n\f$ é o número de
 * retângulos. O resultado é acumulado e multiplicado pela base dos retângulos
 * (\f$\Delta x\f$).
 *
 * @param a Limite inferior da integração.
 * @param func Ponteiro para a função a ser integrada.
 * @param n Número de retângulos.
 * @param dx Largura de cada retângulo.
 * @return O valor aproximado da integral.
 */
static double riemann_dir(double a, Function *func, size_t n, double dx)
{
	double res = 0; // Acumula resultado das somas.

	for (size_t i = 1; i <= n; ++i)
		res += func->eval(a + i * dx, func->impl);

	// Retorna o resultado multiplicado pela base dos retângulos (Δx).
	return res * dx;
}

/**
 * @brief Calcula a soma de Riemann pela esquerda.
 *
 * @details Para cada \f$i\f$ inteiro no intervalo \f$[0, n)\f$, esta função
 * avalia a função arbitrária `func` em \f$x_i = a + i \cdot \Delta x\f$, onde
 * \f$a\f$ é o limite inferior da integração, e \f$n\f$ é o número de
 * retângulos. O resultado é acumulado e multiplicado pela base dos retângulos
 * (\f$\Delta x\f$).
 *
 * @param a Limite inferior da integração.
 * @param func Ponteiro para a função a ser integrada.
 * @param n Número de retângulos.
 * @param dx Largura de cada retângulo.
 * @return O valor aproximado da integral.
 */
static double riemann_esq(double a, Function *func, size_t n, double dx)
{
	double res = 0;

	for (size_t i = 0; i < n; ++i)
		res += func->eval(a + i * dx, func->impl);

	return res * dx;
}