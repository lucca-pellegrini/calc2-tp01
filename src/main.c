#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "riemann.h"

int main(void)
{
	double coefficients[] = { 0, 0, 2 };
	Function *polynomial_ptr = function_new(POLYNOMIAL, 2, coefficients);

	printf("%g\n", riemann(0, 1, polynomial_ptr, 1 << 20, ESQUERDA));

	function_free(polynomial_ptr);
	return EXIT_SUCCESS;
}