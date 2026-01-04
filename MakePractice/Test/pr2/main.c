
#include <stdio.h>
#include <stdlib.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"


int main()
{
    int a = 10;
    int b = 20;

    printf("Addition: %d\n", my_add(a, b));
    printf("Substraction: %d\n", my_sub(a, b));
    printf("Multiplication: %d\n", my_mul(a, b));
    printf("Division: %d\n", my_div(a, b));

    return 0;
}