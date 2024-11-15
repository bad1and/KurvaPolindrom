#include <ncurses.h>
#include <locale.h>
#include <wchar.h>

#define MAX_LEN 100

// Проверка, является ли подстрока палиндромом
int is_palindrome(wchar_t *str, int start, int end) {
    while (start < end) {
        if (str[start] != str[end])
            return 0;
        start++;
        end--;
    }
    return 1;
}

// Функция для нахождения всех палиндромов в строке
void find_palindromes(wchar_t *str) {
    int len = wcslen(str);
    int found = 0;

    printw("Палиндромы и их длина:\n");

    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            if (is_palindrome(str, i, j)) {
                int palindrome_length = j - i + 1;
                if (palindrome_length > 1) { // Игнорировать одиночные символы
                    found = 1;
                    printw("Палиндром: ");
                    for (int k = i; k <= j; k++) {
                        printw("%lc", str[k]);
                    }
                    printw(", длина: %d\n", palindrome_length);
                }
            }
        }
    }

    if (!found) {
        printw("Палиндромы не найдены.\n");
    }
}

int main() {
    setlocale(LC_ALL, ""); // Установка локали для поддержки русского языка
    initscr();             // Инициализация ncurses
    noecho();              // Отключение отображения ввода
    wchar_t input[MAX_LEN + 1];
    int command;

    do {
        clear();
        printw("Введите строку (не более %d символов): ", MAX_LEN);
        echo();
        scanw("%ls", input); // Ввод строки
        noecho();


        find_palindromes(input); // Поиск и вывод палиндромов

        printw("\nНажмите 'q' для выхода, 'e' для редактирования строки или 'Enter' для повтора поиска: ");
        command = getch();

        if (command == 'e') {
            clear();
            printw("Введите новую строку: ");
            echo();
            scanw("%ls", input);
            noecho();
        }

    } while (command != 'q');

    endwin(); // Завершение ncurses
    return 0;
}
