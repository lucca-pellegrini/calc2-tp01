// SPDX-License-Identifier: ISC

/**
 * @file main.c
 * @brief Exemplo de uso das funções de polinômio e soma de Riemann.
 *
 * @details Este arquivo demonstra como criar e manipular uma função
 * polinomial, e como calcular a integral dessa função usando somas de Riemann.
 * O exemplo específico considera quatro polinômios:
 * - a) \f$ \int_{1}^{2} (x^3 + 1) \, dx \f$
 * - b) \f$ \int_{3}^{5} (1 - x^2) \, dx \f$
 * - c) \f$ \int_{7}^{9} x^4 \, dx \f$
 * - d) \f$ \int_{-1}^{1} (x^2 - 1) \, dx \f$
 *
 * O programa calcula a integral desses polinômios nos intervalos especificados
 * utilizando a soma de Riemann pela direita e pela esquerda com diferentes
 * quantidades de retângulos: 100, 300, 600, 1000, 1500 e 2000.
 */

#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "riemann.h"

/**
 * @brief Estrutura que representa os limites de integração.
 */
typedef struct {
	double a; /**< Limite inferior de integração */
	double b; /**< Limite superior de integração */
} Limites;

/**
 * @brief Função principal do programa.
 *
 * @details Esta função define os polinômios e seus limites de integração,
 * calcula as integrais utilizando somas de Riemann pela direita e pela
 * esquerda, e exibe os resultados. Os polinômios considerados são:
 * - a) \f$ \int_{1}^{2} (x^3 + 1) \, dx \f$
 * - b) \f$ \int_{3}^{5} (1 - x^2) \, dx \f$
 * - c) \f$ \int_{7}^{9} x^4 \, dx \f$
 * - d) \f$ \int_{-1}^{1} (x^2 - 1) \, dx \f$
 *
 * @return EXIT_SUCCESS em caso de sucesso.
 */
int main(void)
{
	// Definições das funções e dos limites de cada questão.
	constexpr int num_questoes = 4;
	double coefs[num_questoes][5] = {
		{ 1, 0, 0, 1 }, { 1, 0, -1 }, { 0, 0, 0, 0, 1 }, { -1, 0, 1 }
	};
	const size_t graus[num_questoes] = { 3, 2, 4, 2 };
	const Limites limites[num_questoes] = {
		{ 1, 2 }, { 3, 5 }, { 7, 9 }, { -1, 1 }
	};
	Function *a = function_new(POLYNOMIAL, graus[0], coefs[0]),
		 *b = function_new(POLYNOMIAL, graus[1], coefs[1]),
		 *c = function_new(POLYNOMIAL, graus[2], coefs[2]),
		 *d = function_new(POLYNOMIAL, graus[3], coefs[3]);
	Function *questoes[num_questoes] = { a, b, c, d };

	// Quantidades de retângulos que usaremos em cada iteração.
	constexpr int num_valores = 6;
	const size_t valores[num_valores] = { 100, 300, 600, 1000, 1500, 2000 };

	// Tipos de somas de Riemann que realizaremos.
	constexpr int num_tipos = 2;
	const SumType tipos[num_tipos] = { DIREITA, ESQUERDA };

	// Exibe um cabeçalho com as definições dos polinômios.
	puts("Integraremos os seguintes polinômios:");
	for (int i = 0; i < num_questoes; ++i) {
		double *c = coefs[i];
		size_t g = graus[i];
		Limites l = limites[i];

		printf("%c) ∫(", 'a' + i);
		if (c[0])
			printf("%g + ", c[0]);
		for (size_t j = 1; j < g; ++j)
			if (c[j])
				printf("%gx^%ju + ", c[j], j);
		printf("%gx^%ju)\tentre [%g, %g]\n", c[g], g, l.a, l.b);
	}

	// Executa todas as somas.
	for (int i = 0; i < num_tipos; ++i) {
		SumType t = tipos[i];
		printf("\nCalculando somas de Riemann pela %s:\n",
		       t == DIREITA ? "direita" : "esquerda");

		for (int i = 0; i < num_valores; ++i) {
			size_t n = valores[i];
			printf("n = %4ju:", n);

			for (int i = 0; i < num_questoes; ++i) {
				Function *f = questoes[i];
				Limites l = limites[i];
				double res = riemann(l.a, l.b, f, n, t);

				printf("\t%c) %g", i + 'a', res);
			}

			putchar('\n');
		}
	}

	for (int i = 0; i < num_questoes; ++i)
		function_free(questoes[i]);
	return EXIT_SUCCESS;
}