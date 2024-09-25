#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <errno.h>

// Macro-função auxiliar para verificação de erros. Avalia uma expressão
// arbitrária (`condition`) e, se for verdadeira mostra a mensagem de erro
// `errmsg` em contexto, acompanhada de quaisquer detalhes armazenados no
// código de erro providenciado pelo sistema. Ao final, pula para a linha de
// código designada pelo rótulo `label`.
#define ERRNOCHECK(condition, errmsg, label)                           \
	do {                                                           \
		if (condition) {                                       \
			int errsv = errno;                             \
			fprintf(stderr, "%s:%d:%s: Error: ", __FILE__, \
				__LINE__, __func__);                   \
			errno = errsv;                                 \
			perror((errmsg));                              \
			goto label;                                    \
		}                                                      \
	} while (0)

#endif // !UTIL_H