#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[10];
    char second_name[10];
    char tel[10];
} person;

person* phonebook = NULL;
int count = 0;

void add_abonent() {
    phonebook = realloc(phonebook, (count + 1) * sizeof(person));
    if(phonebook == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    printf("Введите имя абонента: ");
    scanf("%s", phonebook[count].name);
    printf("Введите фамилию абонента: ");
    scanf("%s", phonebook[count].second_name);
    printf("Введите номер телефона абонента: ");
    scanf("%s", phonebook[count].tel);
    count++;
    printf("Абонент добавлен\n");
}

void show_abonent() {
    if (count == 0) {
        printf("Справочник пуст\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("Абонент %d:\n", i + 1);
            printf("Имя: %s\n", phonebook[i].name);
            printf("Фамилия: %s\n", phonebook[i].second_name);
            printf("Телефон: %s\n", phonebook[i].tel);
            printf("\n");
        }
    }
}

void del_abonent(int index) {
    if (index < count && index >= 0) {
        phonebook[index] = phonebook[count - 1];
        count--;
        phonebook = realloc(phonebook, count * sizeof(person));
        if (count > 0 && phonebook == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(1);
        }
        printf("Абонент удален\n");
    } else {
        printf("Неверный индекс абонента\n");
    }
}


void search_abonent() {
    char search_name[100];
    printf("Введите имя абонента: ");
    scanf("%s", search_name);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(phonebook[i].name, search_name) == 0) {
            printf("Абонент найден\n");
            printf("Имя: %s\n", phonebook[i].name);
            printf("Фамилия: %s\n", phonebook[i].second_name);
            printf("Телефон: %s\n", phonebook[i].tel);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Абонент не найден\n");
    }
}

int main() {
    int choice;
    do {
        printf("ГЛАВНОЕ МЕНЮ\n");
        printf("Выберите пункт меню\n");
        printf("1. Добавить абонента\n");
        printf("2. Удалить абонента\n");
        printf("3. Найти абонента\n");
        printf("4. Вывести справочник\n");
        printf("5. Выход\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_abonent();
                break;
            case 2: {
                int index;
                printf("Введите индекс абонента: ");
                scanf("%d", &index);
                del_abonent(index - 1);
                break;
            }
            case 3:
                search_abonent();
                break;
            case 4:
                show_abonent();
                break;
            case 5:
                printf("Выход из программы\n");
                break;
            default:
                printf("Неверный ввод\n");
        }
    } while (choice != 5);

    free(phonebook);
    return 0;
}