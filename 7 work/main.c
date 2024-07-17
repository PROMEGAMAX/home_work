#include <stdio.h>
#include "calc.h"

void print_menu() {
    printf("ГЛАВНОЕ МЕНЮ\n");
    printf("Выберите операцию\n");
    printf("1. Сложение\n");
    printf("2. Вычитание\n");
    printf("3. Умножение\n");
    printf("4. Деление\n");
    printf("5. Выход\n");
}

int main(){
    int choice = 0;
    int num1, num2;

    while(choice != 5){
        print_menu();
        scanf("%d", &choice);
        if (choice == 5){
            printf("Выход из программы\n");
            break;
        }
        printf("Введите два числа: ");
        scanf("%d %d", &num1, &num2);

    switch(choice){
        case 1:
        printf("Результат: %d\n", add(num1, num2));
        break;
        case 2:
        printf("Результат: %d\n", sub(num1, num2));
        break;
        case 3:
        printf("Результат: %d\n", mul(num1, num2));
        break;
        case 4:
        if(num2 != 0){
            printf("Результат: %d\n", div(num1, num2));
            }
        else printf("Ошибка: Деление на ноль\n");
        break;
        default:
        printf("Неверный ввод\n");
    }
}
        return 0;
}