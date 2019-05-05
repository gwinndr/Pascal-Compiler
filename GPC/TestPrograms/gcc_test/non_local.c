#include <stdio.h>
#include <stdlib.h>

int x;

void test()
{
    // int x;
    for(x = 0; x < 10; x = x+1)
    {
        printf("%d\n", x);
    }
}

int main()
{
    test();
}
