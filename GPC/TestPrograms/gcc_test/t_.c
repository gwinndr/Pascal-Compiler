#include <stdio.h>
#include <stdlib.h>

void test()
{
    int y;

    y = 12;
    printf("%d\n", y);
}

void test2()
{
    int y;

    y = 28;
    printf("%d\n", y);
}

int main()
{
    int x;

    x = 13;
    printf("%d\n", x);

    test();
    test2();
}
