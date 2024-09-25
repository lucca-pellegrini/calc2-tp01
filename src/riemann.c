#include <stdio.h>

#include "riemann.h"
#include "func.h"

static double riemann_esq(double a, Function *func, size_t n, double dx);
static double riemann_dir(double a, Function *func, size_t n, double dx);

double riemann(double a, double b, Function *func, size_t n, SumType t)
{
	double dx = (b - a) / n;

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

static double riemann_esq(double a, Function *func, size_t n, double dx)
{
	double sum = 0;

	for (size_t i = 0; i < n; ++i)
		sum += func->eval(a + i * dx, func->impl);

	return sum * dx;
}

static double riemann_dir(double a, Function *func, size_t n, double dx)
{
	double sum = 0;

	for (size_t i = 1; i <= n; ++i)
		sum += func->eval(a + i * dx, func->impl);

	return sum * dx;
}