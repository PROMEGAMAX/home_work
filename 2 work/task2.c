#include <stdio.h>
#include <stdlib.h>

int main(){
    int N;
    int number[N];

    printf("Введите размер массива: ");
    scanf("%d", &N);

    printf("Введите %d элементов массива:\n", N);
    for(int i = 0; i < N; i++){
        scanf("%d", &number[i]);
    }

    printf("Массив в обратном порядке:\n");
    for(int i = N - 1; i >= 0; i--){
        printf("%d ", number[i]);
    }
    printf("\n");

    return 0;
}
