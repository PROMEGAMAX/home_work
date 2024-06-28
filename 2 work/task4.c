#include <stdio.h>
#include <stdlib.h>

void fillSpiral(int arraySize, int matrix[arraySize][arraySize]) {
    int value = 1;
    int top = 0, bottom = arraySize - 1;
    int left = 0, right = arraySize - 1;
    while(value <= arraySize * arraySize){
        
        for (int j = left; j <= right; j++) {
            matrix[top][j] = value++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = value++;
        }
        right--;

        for (int j = right; j >= left; j--) {
            matrix[bottom][j] = value++;
        }
        bottom--;

        for (int i = bottom; i >= top; i--) {
            matrix[i][left] = value++;
        }
        left++;
    }
}

int main() {
    int arraySize;
    printf("Введите размер матрицы N: ");
    scanf("%d", &arraySize);

    int matrix[arraySize][arraySize];
    fillSpiral(arraySize, matrix);

    printf("Спиральная матрица %d x %d:\n", arraySize, arraySize);
        for(int i = 0; i < arraySize; i++){
        for(int j = 0; j < arraySize; j++){
         printf("%d\t", matrix[i][j]);   
        }
    printf("\n");
    }

    return 0;
}