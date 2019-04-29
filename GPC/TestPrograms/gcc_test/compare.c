#include <stdio.h>
#include <stdlib.h>

void test()
{
    int x, y, z;

    x = 13;
    y = 12;
    z = 14;
    if(x > y && z > x)
    {
        printf("%d\n", x);
    }

    // if(y > x)
    // {
    //     printf("%d\n", y);
    // }
}

int main()
{
    test();
}
