#include <stdio.h>
#include <stdlib.h>

int main(){
    int N;
    int matrix[100][100];
    int value = 1;

    printf("Введите N\n");
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
        matrix[i][j] = value++;
    }
}

    printf("Матрица %d x %d:\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}