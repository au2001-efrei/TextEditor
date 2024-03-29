#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "main.h"
#include "yeet.h"
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
    editor.clipboard.length = 0;
    editor.clipboard.first = NULL;
    editor.clipboard.last = NULL;
    editor.x = 0;
    editor.y = 0;
    editor.saved = true;

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
    int yaatd = 0;
    while (running) {
        editor_display(editor);
        refresh();

        int key = getch();

        switch (key) {
        case -1: // No key
            break;

        case 3: // Ctrl-C
            {
                char *line = string_get_line(editor->string, editor->y);
                string_free(&editor->clipboard);
                editor->clipboard = string_from_char_array(line);
                free(line);
            }
            break;

        case 4: // Ctrl-D
            {
                char *line = string_get_line(editor->string, editor->y);
                string_concatenate(&editor->string, line, string_get_offset(editor->string, editor->y));
                ++editor->y;
                editor->saved = false;
                yeet(editor, line);
                free(line);
            }
            break;

        case 6: // Ctrl-F
            {
                char *labels[] = { " Search text: " };
                String *inputs = editor_input(editor, labels, 1);

                if (inputs != NULL) {
                    int offset = string_get_offset(editor->string, editor->y) + editor->x + 1;
                    if (offset >= editor->string.length) {
                        offset = 0;
                        editor->y = 0;
                        editor->x = -1;
                    }

                    char *search = string_to_char_array(inputs[0]);
                    string_free(&(inputs[0]));
                    free(inputs);

                    int length = strlen(search);
                    int result = string_search(editor->string, search, offset);

                    if (result >= 0) {
                        editor->x += result + length + 1;
                        int length = string_get_line_length(editor->string, editor->y) + 1;
                        while (editor->x >= length) {
                            ++editor->y;
                            editor->x -= length;
                            length = string_get_line_length(editor->string, editor->y) + 1;
                        }

                        yoot(editor, search);
                    } else {
                        editor->y = string_get_line_count(editor->string) - 1;
                        editor->x = string_get_line_length(editor->string, editor->y);

                        editor_display(editor);

                        char *labels[] = { " No match left." };
                        String inputs[] = { { 0, NULL, NULL } };
                        editor_display_input(editor, labels, 1, inputs, 0, 0);
                        move(LINES - 1, COLS - 1);
                        refresh();
                        sleep(1);
                    }

                    free(search);
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
                    editor->saved = true;

                    string_free(&(inputs[0]));
                    free(inputs);

                    int lines = string_get_line_count(editor->string) - 1;
                    if (editor->y > lines) editor->y = lines;
                    int length = string_get_line_length(editor->string, editor->y);
                    if (editor->x > length) editor->x = length;
                }
            }
            break;

        case 16: // Ctrl-P
            {
                char *line = string_get_line(editor->string, editor->y);
                String reversed = string_from_char_array(line);
                free(line);

                string_reverse(&reversed);
                string_pop(&reversed, 0);

                int position = string_get_offset(editor->string, editor->y);
                free(string_delete(&editor->string, position, reversed.length));
                string_concatenate_string(&editor->string, reversed, position);
                string_free(&reversed);

                editor->saved = false;
            }
            break;

        case 17: // Ctrl-Q
            if (editor_check_saved(editor))
                running = false;
            break;

        case 18: // Ctrl-R
            {
                char *labels[] = { " Replace text: ", "    With text: " };
                String *inputs = editor_input(editor, labels, 2);

                if (inputs != NULL) {
                    if (inputs[0].length > 0) {
                        char *search = string_to_char_array(inputs[0]);
                        char *replacement = string_to_char_array(inputs[1]);
                        string_free(&(inputs[0]));
                        string_free(&(inputs[1]));
                        free(inputs);

                        int result = string_replace(&editor->string, search, replacement);
                        free(search);

                        int lines = string_get_line_count(editor->string) - 1;
                        if (editor->y > lines) editor->y = lines;
                        int length = string_get_line_length(editor->string, editor->y);
                        if (editor->x > length) editor->x = length;

                        editor_display(editor);

                        char *labels[1];
                        if (result > 0) {
                            int i = result, ilength = 1;
                            while (i >= 10) {
                                i /= 10;
                                ++ilength;
                            }

                            labels[0] = (char *) malloc(sizeof(char) * (19 + ilength));
                            sprintf(labels[0], " %d matches replaced.", result);
                            editor->saved = false;
                        } else {
                            labels[0] = " No match left to replace.";
                        }

                        String inputs[] = { { 0, NULL, NULL } };
                        editor_display_input(editor, labels, 1, inputs, 0, 0);
                        move(LINES - 1, COLS - 1);
                        refresh();
                        sleep(1);
                    } else {
                        string_free(&(inputs[0]));
                        string_free(&(inputs[1]));
                        free(inputs);

                        editor_display(editor);

                        char *labels[] = { " Couldn't replace empty string." };
                        String inputs[] = { { 0, NULL, NULL } };
                        editor_display_input(editor, labels, 1, inputs, 0, 0);
                        move(LINES - 1, COLS - 1);
                        refresh();
                        sleep(1);
                    }
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
            editor->saved = true;
            break;

        case 22: // Ctrl-V
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_concatenate_string(&editor->string, editor->clipboard, position);

                editor->x += editor->clipboard.length;
                int length = string_get_line_length(editor->string, editor->y) + 1;
                while (editor->x >= length) {
                    ++editor->y;
                    editor->x -= length;
                    length = string_get_line_length(editor->string, editor->y) + 1;
                }

                editor->saved = false;
            }
            break;

        case 23: // Ctrl-W
            if (editor_check_saved(editor)) {
                if (editor->file != NULL) {
                    free(editor->file);
                    editor->file = NULL;
                }

                string_free(&editor->string);
                editor->x = 0;
                editor->y = 0;
                editor->saved = true;
            }
            break;

        case 24: // Ctrl-X
            {
                char *line = string_get_line(editor->string, editor->y);
                string_free(&editor->clipboard);
                string_concatenate(&editor->clipboard, line, 0);
                free(line);

                int position = string_get_offset(editor->string, editor->y);
                free(string_delete(&editor->string, position, editor->clipboard.length));

                editor->x = 0;
                editor->saved = false;
            }
            break;

        case 27: // Escape
            break;

        case 127: // Backspace (macOS)
        case 263: // Backspace (Linux)
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                if (position > 0) {
                    int length = string_get_line_length(editor->string, editor->y);
                    if (string_pop(&editor->string, position - 1) == '\n') {
                        --editor->y;
                        editor->x = string_get_line_length(editor->string, editor->y) - length;
                    } else --editor->x;
                    editor->saved = false;
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
                if (string_pop(&editor->string, position) != '\0')
                    editor->saved = false;
            }
            break;

        case 410: // Resize
            break;

        case '\n':
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_insert(&editor->string, (char) key, position);

                ++editor->y;
                editor->x = 0;
                editor->saved = false;
            }
            break;

        case '\t':
            {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                do {
                    string_insert(&editor->string, ' ', position++);
                    ++editor->x;
                } while (editor->x % 4 != 0);
                editor->saved = false;
            }
            break;

        default:
            if ((key >= 32 && key <= 126) || (key >= 128 && key <= 255)) {
                int position = string_get_offset(editor->string, editor->y) + editor->x;
                string_insert(&editor->string, (char) key, position);
                ++editor->x;
                editor->saved = false;
            } else {
                clear();
                printw("Unknow key (code %d): %c\n", key, key);
                refresh();
                sleep(1);
            }
        }

        yaat(&yaatd, key);
    }
}

void quit(int error) {
    attroff(A_BOLD);
    endwin();
    exit(error);
}
