#include <stdio.h>
#include <stdlib.h>

void test()
{
    int x;

    x = 0;
    while(x < 10)
    {
        printf("%d\n", x);
        x = x + 1;
    }
}

int main()
{
    test();
}
