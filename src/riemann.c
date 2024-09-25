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

// Implementação da soma de Riemann pela direita. Para cada `i` no intervalo
// [1, n], invocamos o método `eval` da função `func`, assim avaliando a função
// arbitrária `func` em um x = a + i·Δx, onde `a` é o limite inicial da
// integração, e `n` é p número de retângulos.
static double riemann_dir(double a, Function *func, size_t n, double dx)
{
	double res = 0; // Acumula resultado das somas.

	for (size_t i = 1; i <= n; ++i)
		res += func->eval(a + i * dx, func->impl);

	// Retorna o resultado multiplicado pela base dos retângulos (Δx).
	return res * dx;
}

// Implementação da soma de Riemann pela esquerda. Funciona exatamente da mesma
// forma que `riemann_dir()`, exceto que o intervalo é [0, n[.
static double riemann_esq(double a, Function *func, size_t n, double dx)
{
	double res = 0;

	for (size_t i = 0; i < n; ++i)
		res += func->eval(a + i * dx, func->impl);

	return res * dx;
}