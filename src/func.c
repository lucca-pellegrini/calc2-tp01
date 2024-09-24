#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "util.h"

// Definições dos tipos de funções.
typedef struct {
	size_t degree; // Grau do polinômio.
	double *restrict coefficients; // Coeficientes dos termos.
} Polynomial;

// Declarações internas.
static Polynomial *polynomial_new(const size_t degree,
				  const double *restrict coeffs);
static void polynomial_free(Polynomial *restrict p);
double polynomial_eval(const double x, const Polynomial *restrict p);

// Instancia uma função arbitrária a partir de um tipo.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(const FunctionType t, ...)
{
	va_list args;
	Function *restrict f = malloc(sizeof(typeof(*f)));
	ERRNOCHECK(f == NULL, "Falha ao alocar memória para função", cleanup);

	switch (t) {
	case POLYNOMIAL:
		va_start(args, t);
		size_t degrees = va_arg(args, size_t);
		double *coeffs = va_arg(args, double *);
		va_end(args);
		f->func = polynomial_new(degrees, coeffs);
		f->eval = (eval_ptr_t)&polynomial_eval;
		break;
	default:
		fprintf(stderr, "Tipo de função desconhecido: %d\n", t);
		goto cleanup;
	}

	ERRNOCHECK(f->func == NULL, "polynomial_new() é NULL", cleanup);
	f->type = t;
	return f;

cleanup:
	free(f);
	return NULL;
}

// Libera uma função arbitrária.
void function_free(Function *restrict f)
{
	switch (f->type) {
	case POLYNOMIAL:
		polynomial_free(f->func);
		break;
	default:
		fprintf(stderr, "FATAL: impossível liberar %d\n", f->type);
		exit(EXIT_FAILURE);
	}

	free(f);
}

// Métodos dos diferentes tipos de função.

static Polynomial *polynomial_new(const size_t degree,
				  const double *restrict coeffs)
{
	Polynomial *restrict p = malloc(sizeof(Polynomial));
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

static void polynomial_free(Polynomial *restrict p)
{
	free(p->coefficients);
	free(p);
}

double polynomial_eval(const double x, const Polynomial *restrict p)
{
	double acc = 0;

	for (size_t i = 0; i <= p->degree; ++i)
		acc += p->coefficients[i] * pow(x, i);

	return acc;
}