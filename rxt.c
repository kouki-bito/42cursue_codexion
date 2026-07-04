#include <stdio.h>
#include <stdlib.h>

int g = 1, h = 2;

int func(int u, int v)
{
    int w;

    w = u + v + g + h;

    printf("uの番地: %p\n", &u);
    printf("vの番地: %p\n", &v);
    printf("wの番地: %p\n", &w);
    printf("gの番地: %p\n", &g);
    printf("hの番地: %p\n", &h);
    
    return w;
}

int main(void)
{
    int a, b, *p;

    p = (int *) malloc(sizeof(int));
    scanf("%d %d", &a, p);
    b = func(a, *p);

    printf("aの番地: %p\n", &a);
    printf("bの番地: %p\n", &b);
    printf("pの番地: %p\n", &p);
    printf("malloc領域の番地: %p\n", p);
    printf("mainの番地: %p\n", &main);
    printf("funcの番地: %p\n", &func);
    printf("printfの番地: %p\n", &printf);
    printf("scanfの番地: %p\n", &scanf);

    scanf("%d", &a);
    return 0;
}