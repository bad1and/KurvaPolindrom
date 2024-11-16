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

//чек на палиндром
bool isPalindrome(const char *str, int start, int end) {
    while (start < end) {
        // Пропуск пробелов и символов '_'
        if (str[start] == ' ' || str[start] == '_') {
            start++;
            continue;
        }
        if (str[end] == ' ' || str[end] == '_') {
            end--;
            continue;
        }
        // Сравнение символов
        if (str[start] != str[end]) return false;
        start++;
        end--;
    }
    return true; // палиндром
}

// два цикла поиск
void findPalindromes(const char *str) {
    int len = strlen(str);
    clear();
    mvprintw(0, 0, "Найденные палиндромы и их длины:");
    int found = 0;

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) { // длина палика > 1
            if (isPalindrome(str, i, j)) {
                int length = j - i + 1;
                if (length > 1) {
                    char temp[length + 1];
                    int k = 0;
                    for (int p = i; p <= j; p++) {
                        if (str[p] != '_') {
                            temp[k++] = str[p];
                        }
                    }
                    temp[k] = '\0';
                    if (k>1) {
                        mvprintw(found + 1, 0, "Палиндром: %s, Длина: %d", temp, k);
                        found++;
                    }
                }
                break;
            }
        }
    }
    if (found == 0) {
        mvprintw(1, 0, "Палиндромы не найдены.");
    }
    refresh();
    getch();
}

void drawmain(int width, int height, char maze[height][width], int PosX, int PosY) {
    clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == PosX && y == PosY) {
                mvaddch(y, x, '$');
            } else {
                mvaddch(y, x, maze[y][x]);
            }
        }
    }
    refresh();
}

void keywork(int width, int height, char maze[height][width]) {
    key = getch();

    if (key >= 32 && key <= 126) { // Печатаемые символы
        maze[0][PosX] = key;
        if (PosX < width - 1) PosX++;
    } else if (key == 127 || key == KEY_BACKSPACE) {
        if (PosX > 0) {
            PosX--;
            maze[0][PosX] = '_';
        }
    } else if (key == KEY_LEFT) {
        if (PosX > 0) PosX--;
    } else if (key == KEY_RIGHT) {
        if (PosX < width - 1) PosX++;
    }
}

void createlab(int width, int height, char maze[height][width]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x] = '_';
        }
    }
}

void matrixToString(int width, int height, char maze[height][width], char *result) {
    int index = 0;
    for (int x = 0; x < width; x++) {
        result[index++] = maze[0][x];
    }
    result[index] = '\0';
}

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
