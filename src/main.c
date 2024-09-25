/**
 * @file main.c
 * @brief Exemplo de uso das funções de polinômio e soma de Riemann.
 *
 * @details Este arquivo demonstra como criar e manipular uma função polinomial,
 * e como calcular a integral dessa função usando somas de Riemann. O exemplo
 * específico cria um polinômio de grau 2 (\f$f(x) = 2x^2\f$) e calcula sua
 * integral no intervalo \f$[0, 1]\f$ utilizando a soma de Riemann pela esquerda
 * com \f$2^{20}\f$ retângulos.
 */

#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "riemann.h"

int main(void)
{
	// Coeficientes para uma função polinomial:
	// f(x) = 0x^0 + 0x^1 + 2x^2 => f(x) = 2x^2
	double coeficientes[] = { 0, 0, 2 };
	// Instancia um objeto Função de tipo Polinomial de grau 2
	// (quadrático), usando os coeficientes acima.
	Function *polinomio = function_new(POLYNOMIAL, 2, coeficientes);

	// Avalia a soma de Riemann no intervalo [0, 1] da função, usando
	// 1,048,576 (2^{20}) retângulos, amostrando os valores de f(x_i) pela
	// esquerda.
	printf("%g\n", riemann(0, 1, polinomio, 1 << 20, ESQUERDA));

	// Libera todos os recursos alocados para a função polinomial.
	function_free(polinomio);
	return EXIT_SUCCESS;
}