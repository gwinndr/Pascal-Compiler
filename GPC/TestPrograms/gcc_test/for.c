#include <stdio.h>
#include <stdlib.h>

void test()
{
    int x;

    for(x = 0; x < 10; x = x+1)
    {
        printf("%d\n", x);
    }
}

int main()
{
    test();
}
