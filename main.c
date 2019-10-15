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
    editor.x = 0;
    editor.y = 0;

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
            {
                char *line = string_get_line(editor->string, editor->y);
                string_concatenate(&editor->string, line, string_get_offset(editor->string, editor->y));
                free(line);
            }
            break;

        case 127: // Backspace
            if (string_pop_last(&editor->string) == '\n') { // TODO: Remove at cursor
                --editor->y;
                editor->x = string_get_line_length(editor->string, editor->y);
            } else --editor->x;
            break;

        case 258: // Down arrow
            if (editor->y < string_get_line_count(editor->string) - 1) {
                ++editor->y;

                int length = string_get_line_length(editor->string, editor->y);
                if (editor->x > length) editor->x = length;
            }
            break;

        case 259: // Up arrow
            if (editor->y > 0) {
                --editor->y;

                int length = string_get_line_length(editor->string, editor->y);
                if (editor->x > length) editor->x = length;
            }
            break;

        case 260: // Left arrow
            if (editor->x <= 0) {
                if (editor->y > 0) {
                    --editor->y;
                    editor->x = string_get_line_length(editor->string, editor->y);
                }
            } else --editor->x;
            break;

        case 261: // Right arrow
            if (editor->x >= string_get_line_length(editor->string, editor->y)) {
                if (editor->y < string_get_line_count(editor->string) - 1) {
                    ++editor->y;
                    editor->x = 0;
                }
            } else ++editor->x;
            break;

        default:
            string_append(&editor->string, (char) key); // TODO: Insert at cursor

            if (key == '\n') {
                ++editor->y;
                editor->x = 0;
            } else ++editor->x;
        }
    }
}

void quit(int error) {
    endwin();
    exit(error);
}
