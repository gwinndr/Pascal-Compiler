#include <stdio.h>
#include <stdlib.h>

void test()
{
    int x, y, z;

    scanf("%d", &x);
    scanf("%d", &y);
    scanf("%d", &z);

    if(x > y && x > z)
    {
        printf("%d\n", x);
    }
    else if(y > x || y > z)
    {
        printf("%d\n", y);
    }
    else
    {
        printf("%d\n", z);
    }
}

int main()
{
    test();
}
