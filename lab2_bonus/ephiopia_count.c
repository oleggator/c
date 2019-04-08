#include <stdio.h>
#include <stdlib.h>

int mul(int a, int b);

int main(int argc, char **argv) {
    int a = 0, b = 0;
    scanf("%d %d", &a, &b);
    printf("%d\n", mul(a, b));
    return 0;
}

int mul(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    int b_acc = 0;
    while (a != 0) {
        if (a % 2) {
            b_acc += b;
        }

        a /= 2;    
        b *= 2;   
    }

    return b_acc;
}
