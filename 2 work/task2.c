#include <stdio.h> 
#include <stdlib.h> 
 
int main(){ 
    int arraySize; 
     
    printf("Введите размер массива: "); 
    scanf("%d", &arraySize); 
 
    int *number = (int *)malloc(arraySize * sizeof(int)); 
     
    if(number == NULL) { 
        printf("Ошибка выделения памяти\n"); 
        return -1; 
    } 
 
    printf("Введите %d элементов массива:\n", arraySize); 
    for(int i = 0; i < arraySize; i++){ 
        scanf("%d", &number[i]); 
    } 
 
    printf("Массив в обратном порядке:\n"); 
    for(int i = arraySize - 1; i >= 0; i--){ 
        printf("%d ", number[i]); 
    } 
    printf("\n"); 
 
    free(number); 
 
    return 0; 
}