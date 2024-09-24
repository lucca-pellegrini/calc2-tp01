#include <stdio.h>

#include "riemann.h"
#include "func.h"

static double riemann_esq(const double a, Function *restrict func,
			  const size_t n, const double dx);
static double riemann_dir(const double a, Function *restrict func,
			  const size_t n, const double dx);

double riemann(double a, double b, Function *restrict func, size_t n, SumType t)
{
	const double dx = (b - a) / n;

	switch (t) {
	case ESQUERDA:
		return riemann_esq(a, func, n, dx);
	case DIREITA:
		return riemann_dir(a, func, n, dx);
	default:
		fprintf(stderr, "FATAL: tipo de soma inexistente: %d\n", t);
		exit(EXIT_FAILURE);
	}
}

static double riemann_esq(const double a, Function *restrict func,
			  const size_t n, const double dx)
{
	double sum = 0;

	for (size_t i = 0; i < n; ++i)
		sum += func->eval(a + i * dx, func->func);

	return sum * dx;
}

static double riemann_dir(const double a, Function *restrict func,
			  const size_t n, const double dx)
{
	double sum = 0;

	for (size_t i = 1; i <= n; ++i)
		sum += func->eval(a + i * dx, func->func);

	return sum * dx;
}