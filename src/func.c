#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "util.h"

// Definições dos tipos de funções.
typedef struct {
	size_t degree; // Grau do polinômio.
	double *coefficients; // Coeficientes dos termos.
} Polynomial;

// Declarações internas.
static Polynomial *polynomial_new(size_t degree, double *coeffs);
static void polynomial_free(Polynomial *ptr);
extern double polynomial_eval(double x, Polynomial *ptr);

// Instancia uma função arbitrária a partir de um tipo.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(FunctionType type, ...)
{
	va_list args;
	Function *func = malloc(sizeof(typeof(*func)));
	ERRNOCHECK(func == NULL, "Falha ao alocar memória para função",
		   cleanup);

	switch (type) {
	case POLYNOMIAL:
		va_start(args, t);
		size_t degrees = va_arg(args, size_t);
		double *coeffs = va_arg(args, double *);
		va_end(args);
		func->impl = polynomial_new(degrees, coeffs);
		func->eval = (eval_ptr_t)&polynomial_eval;
		break;
	default:
		fprintf(stderr, "Tipo de função desconhecido: %d\n", type);
		goto cleanup;
	}

	ERRNOCHECK(func->impl == NULL, "func->impl é NULL", cleanup);
	func->type = type;
	return func;

cleanup:
	free(func);
	return NULL;
}

// Libera uma função arbitrária.
void function_free(Function *func)
{
	switch (func->type) {
	case POLYNOMIAL:
		polynomial_free(func->impl);
		break;
	default:
		fprintf(stderr, "FATAL: impossível liberar %d\n", func->type);
		exit(EXIT_FAILURE);
	}

	free(func);
}

// Implementação dos diferentes tipos de função.

static Polynomial *polynomial_new(size_t degree, double *coeffs)
{
	Polynomial *p = malloc(sizeof(Polynomial));
	ERRNOCHECK(p == NULL, "Falha ao alocar memória para polinômio", ret);

	p->degree = degree;

	p->coefficients = malloc((degree + 1) * sizeof(double));
	ERRNOCHECK(p->coefficients == NULL,
		   "Falha ao alocar memória para os coeficientes", cleanup);

	memcpy(p->coefficients, coeffs, (degree + 1) * sizeof(typeof(*coeffs)));

	return p;

cleanup:
	free(p);
ret:
	return NULL;
}

static void polynomial_free(Polynomial *ptr)
{
	free(ptr->coefficients);
	free(ptr);
}

extern double polynomial_eval(double x, Polynomial *ptr)
{
	double res = 0;
	double x_pow = 1;

	for (size_t i = 0; i <= ptr->degree; ++i, x_pow *= x)
		res += ptr->coefficients[i] * x_pow;

	return res;
}