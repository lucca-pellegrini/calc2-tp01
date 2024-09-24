#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "riemann.h"

int main(void)
{
	double coefs[] = { 0, 0, 2 };
	Function *p = function_new(POLYNOMIAL, 2, coefs);

	printf("%g\n", riemann(0, 1, p, 1000, ESQUERDA));

	function_free(p);
	return EXIT_SUCCESS;
}