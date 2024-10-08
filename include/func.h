// SPDX-License-Identifier: ISC

/**
 * @file func.h
 * @brief Declarações de tipos e funções para manipulação de funções genéricas.
 *
 * @details Este arquivo contém as declarações de tipos e funções utilizadas
 * para a criação, avaliação e liberação de funções genéricas. As funções
 * genéricas são abstrações que permitem a manipulação de diferentes tipos de
 * funções de maneira uniforme. Este arquivo define os tipos de funções
 * suportadas, a estrutura que encapsula uma função genérica e as funções
 * públicas para manipulação dessas estruturas.
 */

#pragma once
#ifndef FUNC_H
#define FUNC_H

/**
 * @brief Tipo de ponteiro de função para avaliar uma função genérica.
 *
 * @details Este tipo define um ponteiro para uma função que avalia uma função
 * genérica encapsulada em uma estrutura `Function`. A função apontada deve
 * aceitar dois parâmetros:
 * - Um valor do tipo `double`, representando o ponto no qual a função deve ser
 *   avaliada.
 * - Um ponteiro `void *` para o objeto que implementa a função concreta.
 *
 * A função deve retornar um valor `double`, que é o resultado da avaliação da
 * função concreta no ponto especificado.
 *
 * Exemplo de uso para avaliar a função arbitrária `func` em \f$x = 10\f$:
 * @code
 * double resultado = func->eval(10, func->impl);
 * @endcode
 *
 * @param x O ponto no qual a função deve ser avaliada.
 * @param impl Ponteiro para o objeto que implementa a função concreta.
 * @return O valor da função avaliada no ponto especificado.
 */
typedef double (*eval_ptr_t)(double x, void *impl);

/**
 * @brief Tipos de funções que podemos avaliar.
 */
typedef enum {
	POLYNOMIAL /**< Função do tipo polinomial clássico de grau arbitrário. */
} FunctionType;

/**
 * @brief Definição de uma função genérica abstrata.
 *
 * @details Esta estrutura contém uma referência genérica a uma função concreta
 * de um tipo específico. Ela é usada como uma interface para interagir com
 * diferentes implementações de funções. A estrutura `Function` permite que
 * funções de diferentes tipos sejam tratadas de maneira uniforme, encapsulando
 * os detalhes específicos da implementação.
 */
typedef struct Function {
	FunctionType type; /**< Tipo da função, dentre os listados no enum
		            * `FunctionType`. */
	eval_ptr_t eval; /**< Ponteiro para a função que avalia a função
			  * concreta encapsulada. Este ponteiro permite que a
			  * função concreta seja chamada de maneira uniforme,
			  * independentemente de sua implementação. */
	void *impl; /**< Ponteiro para o objeto que implementa a função
		     * concreta. Este ponteiro é um detalhe de implementação e
		     * não deve ser acessado diretamente fora das funções que
		     * manipulam a estrutura `Function`. */
} Function;

// Métodos públicos das funções genéricas. O método `function_new()`, que gera
// funções arbitrárias, recebe o atributo `[[nodiscard]]` para avisar ao
// usuário que ignorar o seu resultado pode causar vazamento de memória.

/**
 * @brief Instancia uma função arbitrária a partir de um tipo e de n
 * parâmetros.
 *
 * @param type O tipo da função.
 * @param ... Parâmetros variáveis dependendo do tipo da função.
 * @return Ponteiro para a nova função criada.
 * @note Ignorar o retorno pode causar vazamento de memória.
 */
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(FunctionType type, ...);

/**
 * @brief Libera uma função arbitrária.
 *
 * @param func Ponteiro para a função a ser liberada.
 */
void function_free(Function *func);

#endif // !FUNC_H