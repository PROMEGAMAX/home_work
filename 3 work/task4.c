#include <stdio.h>
#include <string.h>

char *findSubstring(char *str, char *substr) {
    int len_substr = strlen(substr);

    for (char *p = str; *p != '\0'; p++) {
        if (strncmp(p, substr, len_substr) == 0) {
            return p;
        }
    }
    return NULL;
}

int main() {
    char str[100];
    char substr[100];

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    printf("Введите подстроку: ");
    fgets(substr, sizeof(substr), stdin);
    substr[strcspn(substr, "\n")] = '\0';

    char *result = findSubstring(str, substr);

    if (result != NULL) {
        printf("Позиция подстроки: %ld\n", result - str);
    } else {
        printf("Подстрока не найдена\n");
    }

    return 0;
}
