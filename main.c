#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "main.h"
#include "string.h"
#include "editor.h"

int main(int argc, char *argv[]) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    Editor editor;
    editor.file = NULL;
    editor.string.first = NULL;
    editor.string.last = NULL;

    if (argc > 1) {
        editor.file = argv[1];
        editor.string = string_read_file(editor.file);
    }

    run(&editor);

    quit(0);
    return 0;
}

void run(Editor *editor) {
    bool running = true;
    while (running) {
        editor_display(editor);

        int key = getch();

        switch (key) {
        case 3: // Ctrl-C
        case 17: // Ctrl-Q
        case 24: // Ctrl-X
            running = false;
            break;

        case 19: // Ctrl-S
            if (editor->file != NULL) {
                string_write_file(editor->file, editor->string);
            } else {
                // TODO
            }
            break;

        case 4: // Ctrl-D
            string_append(&editor->string, string_copy(editor->string));
            break;

        case 127: // Backspace
            string_pop_last(&editor->string);
            break;

        default:
            string_append(&editor->string, (char) key);
        }
    }
}

void quit(int error) {
    endwin();
    exit(error);
}
