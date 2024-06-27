#include <stdio.h>

int countOnes(unsigned int n) {
    int count = 0;

    while (n != 0) {
        if (n & 1) {
            count++;
        }
        n >>= 1;
    }
    
    return count;
}

int main() {
    unsigned int number;
    
    printf("Введите целое положительное число: ");
    scanf("%u", &number);
    
    int onesCount = countOnes(number);
    
    printf("Количество единиц в двоичном представлении числа %u: %d\n", number, onesCount);

    return 0;
}
