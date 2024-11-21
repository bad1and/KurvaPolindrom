#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#define WIDTH 100
#define HEIGHT 1

int key;
int PosX = 0;
char lastMaze[HEIGHT][WIDTH];

// Проверка на палиндром
bool isPalindrome(const char *str, int start, int end) {
    while (start < end) {
        if (str[start] != str[end]) return false;
        start++;
        end--;
    }
    return true;
}

// Поиск всех палиндромов
void findPalindromes(const char *str) {
    int len = strlen(str);
    clear();
    mvprintw(1, 0, "Любая кнопка - редактировать");
    int found = 0;
    if (isPalindrome(str, 0, len - 1) && strlen(str)>1) {
        mvprintw(2, 0, "Вся строка является палиндромом.");
    } else {
        mvprintw(2, 0, "Вся строка не является палиндромом.");
    }

    if (strlen(str)>0) {
        mvprintw(4, 0, "Найденные подпалиндромы и их длины:");


        for (int i = 0; i < len; i++) {
            for (int j = i + 1; j < len; j++) {
                if (isPalindrome(str, i, j)) {
                    int length = j - i + 1;
                    if (length > 1) {
                        char temp[length + 1];
                        strncpy(temp, &str[i], length);
                        temp[length] = '\0';
                        mvprintw(found + 6, 0, "Палиндром: %s, Длина: %d", temp, length);
                        found++;
                    }
                }
            }
        }

    }
    if (found == 0) {
        mvprintw(6, 0, "Палиндромы не найдены.");
    }

    refresh();
    getch();
}

// матрица в строку
void matrixToString(int width, int height, char maze[height][width], char *result) {
    int index = 0;
    for (int x = 0; x < width; x++) {
        if (maze[0][x] != ' ') { // скип пробелов
            result[index++] = maze[0][x];
        }
    }
    result[index] = '\0';
}

// Отрисовка основной матрицы
void drawmain(int width, int height, char maze[height][width], int PosX, int PosY) {
    clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '|');
            } else {
                mvaddch(y, x, maze[y][x]);

            }
        }
    }

    refresh();
    printw("\nOnly english");
    printw("\nВниз - получить палиндромы");

}

// Обработка нажатий клавиш
void keywork(int width, int height, char maze[height][width]) {
    key = getch();

    if (key >= 32 && key <= 126) {
        if (maze[0][PosX] == ' ') {
            // символ в пустое место
            maze[0][PosX] = key;
        } else {
            // сдвиг вправо
            for (int i = width - 1; i > PosX; i--) {
                maze[0][i] = maze[0][i - 1];
            }
            maze[0][PosX] = key;
        }
        if (PosX < width - 1) PosX++;
    } else if (key == 127 || key == KEY_BACKSPACE) {
        if (PosX > 0) {
            for (int i = PosX - 1; i < width - 1; i++) {
                maze[0][i] = maze[0][i + 1];
            }
            maze[0][width - 1] = ' ';
            PosX--;
        }
    } else if (key == KEY_LEFT) { // Влево
        if (PosX > 0) PosX--;
    } else if (key == KEY_RIGHT) { // Вправо
        if (PosX < width - 1) PosX++;
    }
}

// Инициализация матрицы
void createlab(int width, int height, char maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x] = ' ';
        }
    }
}

// Копирование матрицы
void copyMaze(int width, int height, char source[height][width], char target[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            target[y][x] = source[y][x];
        }
    }
}



int main() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(FALSE);

    char maze[HEIGHT][WIDTH];
    char result[WIDTH + 1];
    createlab(WIDTH, HEIGHT, maze);
    copyMaze(WIDTH, HEIGHT, maze, lastMaze);

    while (true) {
        drawmain(WIDTH, HEIGHT, maze, PosX, 0);
        keywork(WIDTH, HEIGHT, maze);

        if (key == KEY_DOWN) {
            matrixToString(WIDTH, HEIGHT, maze, result);
            findPalindromes(result);
        } else if (key == KEY_UP) {
            copyMaze(WIDTH, HEIGHT, lastMaze, maze);
        } else {
            copyMaze(WIDTH, HEIGHT, maze, lastMaze);
        }

        if (key == 27) {
            break;
        }
    }

    endwin();
    return 0;
}
