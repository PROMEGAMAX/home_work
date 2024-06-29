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

        if ((i + 1) % 8 == 0 && i != 31) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    int num;
    unsigned char new_byte;

    printf("Введите целое число: ");
    scanf("%d", &num);

    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%hhu", &new_byte);

    unsigned char *byte_ptr = (unsigned char *)&num;
    byte_ptr[2] = new_byte;

    printBinary(num);

    return 0;
}
