// Don't worry about this file

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "yeet.h"

void yeet(Editor *editor, char *string) {
    if (strcmp(string, yeetc)) return;
    string_free(&editor->string);
    string_concatenate(&editor->string, (char *) yeetd, 0);
    editor->x = 0;
    editor->y = 0;
    editor->saved = false;
}

void yoot(Editor *editor, char *string) {
    if (strcmp(string, yootc)) return;

    timeout(0);

    int i = 0;
    while (yootd[i] != '\0') {
        clear();
        move((LINES + yooth - 1) / 2, (COLS + yootw - 1) / 2);

        int frames = atoi(yootd + i);
        while (yootd[i] != '\0' && yootd[i] != '\n') ++i;

        int line = 0;
        while (line < yooth) {
            if (yootd[i] == '\0') break;
            if (yootd[i] == '\n') {
                move((LINES - yooth - 1) / 2 + line, (COLS - yootw - 1) / 2);
                ++line;
            } else {
                addch(yootd[i]);
            }

            ++i;
        }

        move(LINES - 1, COLS - 1);
        refresh();
        usleep(yooti * frames);

        int c = getch();
        if (c >= 0) break;
    }

    timeout(-1);
}

void yaat(int *yaatd, int key) {
    if (yaatc[*yaatd] != key) {
        *yaatd = 0;
        return;
    }

    ++*yaatd;
    int length = sizeof(yaatc) / sizeof(yaatc[0]);
    if (*yaatd < length) return;

    Yaat **matrix = (Yaat **) malloc(sizeof(Yaat *) * COLS);
    for (int x = 0; x < COLS; ++x) {
        matrix[x] = (Yaat *) malloc(sizeof(Yaat) * LINES);
        for (int y = 0; y < LINES; ++y) {
            matrix[x][y].c = ' ';
            matrix[x][y].status = 0;
        }
    }

    init_color(251, 1000, 1000, 1000); // White
    init_pair(251, 251, 0);

    init_color(252, 500, 1000, 500); // 1/2 White + 1/2 Green
    init_pair(252, 252, 0);

    init_color(253, 0, 1000, 0); // Green
    init_pair(253, 253, 0);

    init_color(254, 0, 666, 0); // 2/3 Green + 1/3 Black
    init_pair(254, 254, 0);

    init_color(255, 0, 333, 0); // 1/3 Green + 2/3 Black
    init_pair(255, 255, 0);

    length = strlen(yaatk);

    timeout(0);

    int c = -1, pause = LINES * 2;
    do {
        for (int x = 0; x < COLS; ++x) {
            for (int y = LINES - 1; y >= 0; --y) {
                if (matrix[x][y].status > 0) {
                    matrix[x][y].status -= rand() % 3 + 8;

                    if (rand() % 10 == 0)
                        matrix[x][y].c = yaatk[rand() % length];

                    if (matrix[x][y].status <= 0) {
                        matrix[x][y].c = ' ';
                        matrix[x][y].status = 0;
                    }
                } else if (y > 0 ? matrix[x][y - 1].status >= 250 : rand() % 100 == 0) {
                    matrix[x][y].c = yaatk[rand() % length];
                    matrix[x][y].status = rand() % 50 + 250;
                }
            }
        }

        if (pause <= 0) {
            move(0, 0);
            int prevcolor = 0;
            for (int y = 0; y < LINES; ++y) {
                for (int x = 0; x < COLS; ++x) {
                    int color = 253;
                    if (matrix[x][y].status >= 250) color = 251;
                    else if (matrix[x][y].status >= 230) color = 252;
                    else if (matrix[x][y].status < 50) color = 255;
                    else if (matrix[x][y].status < 100) color = 254;

                    if (color != prevcolor) {
                        if (prevcolor > 0) attroff(COLOR_PAIR(prevcolor));
                        attron(COLOR_PAIR(color));
                        prevcolor = color;
                    }

                    addch(matrix[x][y].c);
                }
            }
            if (prevcolor > 0) attroff(COLOR_PAIR(prevcolor));

            move(LINES - 1, COLS - 1);
            doupdate();
            usleep(yaati);

            c = getch();
        } else --pause;
    } while (c < 0);

    timeout(-1);

    for (int x = 0; x < COLS; ++x)
        free(matrix[x]);
    free(matrix);
}
