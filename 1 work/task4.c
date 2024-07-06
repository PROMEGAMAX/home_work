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
    int number;        
    int newByteValue;   
    int result;       

    printf("Введите целое положительное число: ");
    scanf("%d", &number);
    printf("Введите новое значение для третьего байта (от 0 до 255): ");
    scanf("%d", &newByteValue);

    if (newByteValue < 0 || newByteValue > 255) {
        printf("Ошибка: новое значение для третьего байта должно быть от 0 до 255.\n");
        return 1;
    }
    result = number & 0xFF00FFFF;
    result = result | (newByteValue << 16);

    printf("Исходное число с измененным третьим байтом: ");
    printBinary(result);

    return 0;
}
