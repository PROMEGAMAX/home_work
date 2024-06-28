#include <stdio.h>
#include <stdlib.h>

int main(){
    int arraySize;
    int matrix[100][100];
    int value = 1;

    printf("Введите N\n");
    scanf("%d", &arraySize);

    for(int i = 0; i < arraySize; i++){
        for(int j = 0; j < arraySize; j++){
            if(j >= arraySize - 1 - i){
                matrix[i][j] = 1;
            } else{
                matrix[i][j] = 0;
                }
            }
        }

    printf("Матрица %d x %d:\n", arraySize, arraySize);
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
