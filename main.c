#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "main.h"
#include "string.h"
#include "editor.h"

int main(int argc, char *argv[]) {
    initscr();
    start_color();
    attron(A_BOLD);

    raw();
    keypad(stdscr, TRUE);
    noecho();

    Editor editor;
    editor.file = NULL;
    editor.string.length = 0;
    editor.string.first = NULL;
    editor.string.last = NULL;
    editor.x = 0;
    editor.y = 0;

    if (argc > 1) {
        editor.file = (char *) malloc(sizeof(char) * (strlen(argv[1]) + 1));
        strcpy(editor.file, argv[1]);
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
        case 4: // Ctrl-D
            {
                char *line = string_get_line(editor->string, editor->y);
                string_concatenate(&editor->string, line, string_get_offset(editor->string, editor->y));
                free(line);
            }
            break;

        case 6: // Ctrl-F
            {
                char *labels[] = { " Search text: " };
                String *inputs = editor_input(editor, labels, 1);

                if (inputs != NULL) {
                    // TODO

                    string_free(&(inputs[0]));
                    free(inputs);
                }
            }
            break;

        case 15: // Ctrl-O
            {
                char *labels[] = { " Open file: " };
                String *inputs = editor_input(editor, labels, 1);

                if (inputs != NULL) {
                    if (editor->file != NULL) free(editor->file);
                    editor->file = string_to_char_array(inputs[0]);
                    editor->string = string_read_file(editor->file);

                    string_free(&(inputs[0]));
                    free(inputs);
                }
            }
            break;

        case 17: // Ctrl-Q
            running = false;
            break;

        case 18: // Ctrl-R
            {
                char *labels[] = { " Replace text: ", "    With text: " };
                String *inputs = editor_input(editor, labels, 2);

                if (inputs != NULL) {
                    // TODO

                    string_free(&(inputs[0]));
                    string_free(&(inputs[1]));
                    free(inputs);
                }
            }
            break;

        case 19: // Ctrl-S
            if (editor->file == NULL) {
                char *labels[] = { " Path to file to save: " };
                String *inputs = editor_input(editor, labels, 1);

                if (inputs != NULL) {
                    editor->file = string_to_char_array(inputs[0]);

                    string_free(&(inputs[0]));
                    free(inputs);
                } else break;
            }

            string_write_file(editor->file, editor->string);
            break;

        case 27: // Escape
            break;

        case 127: // Backspace (macOS)
        case 263: // Backspace (Linux)
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                if (position > 0) {
                    if (string_pop(&editor->string, position - 1) == '\n') {
                        --editor->y;
                        editor->x = string_get_line_length(editor->string, editor->y);
                    } else --editor->x;
                }
            }
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

        case 330: // Delete
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_pop(&editor->string, position);
            }
            break;

        case '\n':
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_insert(&editor->string, (char) key, position);

                ++editor->y;
                editor->x = 0;
            }
            break;

        default:
            if ((key >= 32 && key <= 126) || (key >= 128 && key <= 255)) {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_insert(&editor->string, (char) key, position);
                ++editor->x;
            } else {
                clear();
                printw("Unknow key (code %d): %c\n", key, key);
                refresh();
                sleep(1);
            }
        }
    }
}

void quit(int error) {
    endwin();
    exit(error);
}
