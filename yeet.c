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
