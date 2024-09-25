#pragma once
#ifndef FUNC_H
#define FUNC_H

// Tipo de ponteiro de função para avaliar uma função em x.
typedef typeof(double (*)(double, void *)) eval_ptr_t;

// Tipos de funções que podemos avaliar.
typedef enum { POLYNOMIAL } FunctionType;

// Definição de uma função genérica.
typedef struct {
	FunctionType type; // Tipo da função, dentre os listados acima.
	eval_ptr_t eval; // Ponteiro ao método que avalia esse tipo de função.
	void *impl; // Ponteiro ao objeto privado que implementa a função em si.
} Function;

// Métodos públicos das funções genéricas. O método `function_new()`, que gera
// funções arbitrárias, recebe o atributo `[[nodiscard]]` para avisar ao
// usuário que ignorar o seu resultado pode causar vazamento de memória.
[[nodiscard("Ignorar retorno pode causar vazamento de memória")]]
Function *function_new(FunctionType type, ...);
void function_free(Function *func);

#endif // !FUNC_H