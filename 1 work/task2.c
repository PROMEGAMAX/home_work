#include <stdio.h>

void printBinary(int n) {
    unsigned int mask = 1 << 31;
    
    for (int i = 0; i < 32; i++) {
        if (n & mask) {
            printf("1");
        } else {
            printf("0");
        }
        mask >>= 1;
            if (i % 8 == 0) {
            printf(" ");
        }
    }
        printf("\n");
    }

int main() {
    int number;
    
    printf("Введите целое число: ");
    scanf("%d", &number);
    
    printf("Двоичное представление числа %d: ", number);
    printBinary(number);
    
    return 0;
}
