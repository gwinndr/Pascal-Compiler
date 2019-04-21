#include <stdio.h>
#include <stdlib.h>

void read()
{
    int x;

    scanf("%d", &x);
}

void write()
{
    int y;

    y = 5;
    printf("%d\n", y);
}

int main()
{
    int x, y;

    read();
    write();
}
