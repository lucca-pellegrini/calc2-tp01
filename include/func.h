#pragma once
#ifndef FUNC_H
#define FUNC_H

// Tipos de funções que podemos avaliar.
typedef enum {
	POLYNOMIAL
} FunctionType;

// Tipo de ponteiro de função para avaliar uma função em x.
typedef typeof(double (*)(const double, void *restrict)) eval_ptr_t;

// Definição de uma função genérica.
typedef struct {
	FunctionType type;
	eval_ptr_t eval;
	void *func;
} Function;

// Métodos das funções genéricas.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(const FunctionType t, ...);
void function_free(Function *restrict f);

#endif // !FUNC_H