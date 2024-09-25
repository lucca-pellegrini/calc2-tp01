#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "riemann.h"

int main(void)
{
	// Coeficientes para uma função polinomial:
	// f(x) = 0x⁰ + 0x¹ + 2x² => f(x) = 2x²
	double coeficientes[] = { 0, 0, 2 };
	// Instancia um objeto Função de tipo Polinomial de grau 2
	// (quadrático), usando os coeficientes acima.
	Function *polinomio = function_new(POLYNOMIAL, 2, coeficientes);

	// Avalia a soma de riemann no intervalo [0, 1] da função, usando
	// 1,048,576 (2²⁰) retângulos, amostrando os valores de f(x_i) pela
	// esquerda.
	printf("%g\n", riemann(0, 1, polinomio, 1 << 20, ESQUERDA));

	// Libera todos os recursos alocados para a função polinomial.
	function_free(polinomio);
	return EXIT_SUCCESS;
}