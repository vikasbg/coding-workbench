#include <stdio.h>
#include <stdlib.h>
#include "header.h"
// #include <header.h>

int main() 
{
    int a = 20;
    int b = 30;
    int c = add(20, 30);
    print(c);

    int d = sub(40, 20);
    print(d);
    return 0;
}
