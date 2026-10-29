#include <stdio.h>
#include <string.h>
int main(void) {
    char digits[] = "0123456789";
    int rank = 999999;
    int factorial[10] = {1};
    for (int i = 1; i < 10; ++i) factorial[i] = factorial[i - 1] * i;
    for (int remaining = 10; remaining > 0; --remaining) {
        int index = rank / factorial[remaining - 1];
        rank %= factorial[remaining - 1];
        putchar(digits[index]);
        memmove(digits + index, digits + index + 1, remaining - index);
    }
    putchar('\n');
    return 0;
}
