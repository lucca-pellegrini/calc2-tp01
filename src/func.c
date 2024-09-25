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
static void polynomial_free(Polynomial *p);
extern double polynomial_eval(double x, Polynomial *p);

// Instancia uma função arbitrária a partir de um tipo.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(FunctionType t, ...)
{
	va_list args;
	Function *f = malloc(sizeof(typeof(*f)));
	ERRNOCHECK(f == NULL, "Falha ao alocar memória para função", cleanup);

	switch (t) {
	case POLYNOMIAL:
		va_start(args, t);
		size_t degrees = va_arg(args, size_t);
		double *coeffs = va_arg(args, double *);
		va_end(args);
		f->impl = polynomial_new(degrees, coeffs);
		f->eval = (eval_ptr_t)&polynomial_eval;
		break;
	default:
		fprintf(stderr, "Tipo de função desconhecido: %d\n", t);
		goto cleanup;
	}

	ERRNOCHECK(f->impl == NULL, "f->impl é NULL", cleanup);
	f->type = t;
	return f;

cleanup:
	free(f);
	return NULL;
}

// Libera uma função arbitrária.
void function_free(Function *f)
{
	switch (f->type) {
	case POLYNOMIAL:
		polynomial_free(f->impl);
		break;
	default:
		fprintf(stderr, "FATAL: impossível liberar %d\n", f->type);
		exit(EXIT_FAILURE);
	}

	free(f);
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

static void polynomial_free(Polynomial *p)
{
	free(p->coefficients);
	free(p);
}

extern double polynomial_eval(double x, Polynomial *p)
{
	double acc = 0;
	double x_pow = 1;

	for (size_t i = 0; i <= p->degree; ++i, x_pow *= x)
		acc += p->coefficients[i] * x_pow;

	return acc;
}