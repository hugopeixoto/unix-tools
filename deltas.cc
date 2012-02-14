#include <stdio.h>

int main ()
{
    int prev = 0;
    int n;

    while (scanf("%d", &n) == 1) {
        printf("%d\n", n - prev);
        prev = n;
    }

    return 0;
}
