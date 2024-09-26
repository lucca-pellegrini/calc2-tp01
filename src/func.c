// SPDX-License-Identifier: ISC

/**
 * @file func.c
 * @brief Implementações das funções genéricas e específicas de polinômios.
 *
 * @details Este arquivo contém as implementações das funções genéricas e
 * específicas para manipulação de funções abstratas e polinômios. Ele inclui
 * a criação, avaliação e liberação de funções genéricas, bem como as
 * implementações específicas para funções do tipo polinômio. A estrutura
 * `Function` é utilizada para encapsular diferentes tipos de funções, e as
 * funções aqui definidas permitem a manipulação dessas estruturas de maneira
 * uniforme.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "util.h"

// Definições dos tipos de funções.

/**
 * @brief Estrutura que define um polinômio clássico de grau arbitrário.
 *
 * @details A estrutura `Polynomial` é usada para representar um polinômio de
 * grau arbitrário. Os coeficientes dos termos do polinômio são armazenados em
 * ordem crescente de grau. Isso significa que o coeficiente do termo constante
 * (grau 0) vem primeiro, seguido pelo coeficiente do termo de grau 1, e assim
 * por diante.
 *
 * Por exemplo, para um polinômio \f$f(x) = 1 + 2x^2 + 3x^4\f$, o grau será 4,
 * e o arranjo dos coeficientes será: \f$\{1, 0, 2, 0, 3\}\f$.
 */
typedef struct {
	size_t degree; /**< O grau do polinômio, que é o maior expoente com um
                        * coeficiente não zero. */
	double *coefficients; /**< Um ponteiro para um array de coeficientes
			       * dos termos do polinômio, armazenados em ordem
			       * crescente de grau. O comprimento deste array é
			       * \f$\text{degree} + 1\f$. */
} Polynomial;

// Declarações internas.
static Polynomial *polynomial_new(size_t degree, double *coeffs);
static void polynomial_free(Polynomial *ptr);
extern double polynomial_eval(double x, Polynomial *ptr);

// Instancia uma função arbitrária a partir de um tipo e de n parâmetros.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(FunctionType type, ...) // Recebe n + 1 parâmetros.
{
	va_list args; // Lista de parâmetros dinâmicos da função.

	// Aloca memória para o objeto função e verifica se houve erro.
	Function *func = malloc(sizeof(typeof(*func)));
	ERRNOCHECK(func == nullptr, "Falha ao alocar memória para função",
		   cleanup);

	switch (type) {
	case POLYNOMIAL: // Se o parâmetro `type` for `POLYNOMIAL`:
		va_start(args, type); // Inicializa os parâmetros.
		size_t degrees = va_arg(args, size_t); // Extrai 1º parâmetro.
		double *coeffs = va_arg(args, double *); // Extrai o 2º.
		va_end(args); // Libera a lista de parâmetros.

		// Define a implementação da função como um objeto “polinômio”.
		func->impl = polynomial_new(degrees, coeffs);
		// Define o endereço da função que avalia o polinômio.
		func->eval = (eval_ptr_t)&polynomial_eval;
		break;
	default: // Se `type` for desconhecido:
		fprintf(stderr, "Tipo de função desconhecido: %d\n", type);
		goto cleanup; // Pula para a seção `cleanup` abaixo.
	}

	// Verifica se houve erro ao instanciar o objeto implementação.
	ERRNOCHECK(func->impl == nullptr, "func->impl é NULL", cleanup);

	func->type = type; // Caso contrário, define o tipo da função.
	return func; // Retorna a referência ao objeto.

cleanup: // Se algum erro de alocação ocorreu, libera os recursos e retorna.
	free(func);
	return nullptr;
}

// Libera uma função arbitrária.
void function_free(Function *func)
{
	// Invoca o método de liberação apropriado ao tipo da função.
	switch (func->type) {
	case POLYNOMIAL:
		polynomial_free(func->impl);
		break;
	default: // Se o tipo for desconhecido, precisamos lançar erro fatal.
		fprintf(stderr, "FATAL: impossível liberar %d\n", func->type);
		exit(EXIT_FAILURE); // Termina programa com código de erro.
	}

	free(func);
}

// Implementação dos diferentes tipos de função.
// Funções de tipo polinômio:

/**
 * @brief Instancia um objeto que implementa uma função de tipo polinômio.
 *
 * Recebe um grau (número natural) e um arranjo com os coeficientes, em ordem
 * crescente. Ex.: Para um \f$f(x) = 1 + 2x^2 + 3x^4\f$, o grau será 4, e o
 * arranjo será: \f${1, 0, 2, 0, 3}\f$.
 *
 * @param degree O grau do polinômio.
 * @param coeffs Arranjo com os coeficientes do polinômio. Seu tamanho deve ser
 * no mínimo \f$\left(\text{grau}- 1\right)\f$.
 * @return Ponteiro para o objeto polinômio criado.
 */
static Polynomial *polynomial_new(size_t degree, double *coeffs)
{
	// Aloca memória para o objeto e verifica se houve erro.
	Polynomial *p = malloc(sizeof(Polynomial));
	ERRNOCHECK(p == nullptr, "Falha ao alocar memória para polinômio", ret);

	p->degree = degree; // Define o grau do polinômio.

	// Aloca memória para o arranjo de coeficientes e verifica erro.
	p->coefficients = malloc((degree + 1) * sizeof(double));
	ERRNOCHECK(p->coefficients == nullptr,
		   "Falha ao alocar memória para os coeficientes", cleanup);

	// Eficientemente copia os coeficientes recebidos para o objeto.
	memcpy(p->coefficients, coeffs, (degree + 1) * sizeof(typeof(*coeffs)));

	return p; // Retorna a referência ao objeto.

cleanup:
	free(p);
ret:
	return nullptr;
}

/**
 * @brief Libera um objeto que implementa uma função de tipo polinômio.
 *
 * @param ptr Ponteiro para o objeto polinômio a ser liberado.
 */
static void polynomial_free(Polynomial *ptr)
{
	free(ptr->coefficients); // Libera arranjo dos coeficientes.
	free(ptr); // Libera o objeto em si.
}

/**
 * @brief Avalia um dado objeto polinômio passado pela referência `ptr` em um
 * dado `x`.
 *
 * @param x O valor em que o polinômio será avaliado.
 * @param ptr Ponteiro para o objeto polinômio.
 * @return O valor do polinômio avaliado em `x`.
 * @note Essa função é definida com vinculação externa pois não é invocada
 * nessa unidade de compilação.
 */
extern double polynomial_eval(double x, Polynomial *ptr)
{
	double res = 0; // Valor de resposta a calcular.
	double x_pow = 1; // Armazenará cada potência de `x` até o grau máximo.

	// Itera sobre o espaço de inteiros entre 0 e o grau do polinômio,
	// computando as potências e as adicionando ao valor de resposta.
	for (size_t i = 0; i <= ptr->degree; ++i, x_pow *= x)
		res += ptr->coefficients[i] * x_pow;

	return res; // Retorna a resposta calculada.
}