#include <stdio.h>

#include "riemann.h"
#include "func.h"

static double riemann_esq(double a, Function *func, size_t n, double dx);
static double riemann_dir(double a, Function *func, size_t n, double dx);

double riemann(double min, double max, Function *func, size_t num, SumType type)
{
	double dx = (max - min) / num;

	switch (type) {
	case ESQUERDA:
		return riemann_esq(min, func, num, dx);
	case DIREITA:
		return riemann_dir(min, func, num, dx);
	default:
		fprintf(stderr, "FATAL: tipo de soma inexistente: %d\n", type);
		exit(EXIT_FAILURE);
	}
}

static double riemann_esq(double a, Function *func, size_t n, double dx)
{
	double res = 0;

	for (size_t i = 0; i < n; ++i)
		res += func->eval(a + i * dx, func->impl);

	return res * dx;
}

static double riemann_dir(double a, Function *func, size_t n, double dx)
{
	double res = 0;

	for (size_t i = 1; i <= n; ++i)
		res += func->eval(a + i * dx, func->impl);

	return res * dx;
}