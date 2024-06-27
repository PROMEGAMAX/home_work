#include <stdio.h>
#include <stdlib.h>

void fillSpiral(int N, int matrix[N][N]) {
    int value = 1;
    int top = 0, bottom = N - 1;
    int left = 0, right = N - 1;
    while(value <= N * N){
        
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
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);

    int matrix[N][N];
    fillSpiral(N, matrix);

    printf("Спиральная матрица %d x %d:\n", N, N);
        for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
         printf("%d\t", matrix[i][j]);   
        }
    printf("\n");
    }

    return 0;
}