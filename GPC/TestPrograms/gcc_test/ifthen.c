#include <stdio.h>
#include <stdlib.h>

void test()
{
    int x, y;

    scanf("%d", &x);
    scanf("%d", &y);

    if(x > y)
    {
        printf("%d\n", x);
    }
    else
    {
        printf("%d\n", y);
    }
}

int main()
{
    test();
}
