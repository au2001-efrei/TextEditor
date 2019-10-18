#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "editor.h"
#include "string.h"

void editor_display(Editor *editor) {
    clear();

    int scrollX = 0, scrollY = 0;
    while (editor->y - scrollY > LINES * 3 / 4)
        scrollY += LINES / 2;
    while (editor->x - scrollX > COLS * 3 / 4)
        scrollX += COLS / 2;

    int x = 0, y = 0;
    Character *current = editor->string.first;
    while (current != NULL) {
        if (current->data == '\n') {
            ++y;
            x = 0;
        } else {
            if (y >= scrollY && y < scrollY + LINES && x >= scrollX && x < scrollX + COLS)
                mvaddch(y - scrollY, x - scrollX, current->data);
            ++x;
        }
        current = current->next;
    }

    move(editor->y - scrollY, editor->x - scrollX);
}

void editor_display_input(Editor *editor, char **labels, int lines, String *inputs, int y, int x) {
    int scrollX = 0, width = COLS - strlen(labels[y]);
    while (x - scrollX > width * 3 / 4)
        scrollX += width / 2;

    for (int i = 0; i < lines; ++i) {
        int ly = LINES - lines + i, lx = 0;

        attron(A_REVERSE);

        mvaddstr(ly, lx, labels[i]);
        lx += strlen(labels[i]);

        if (inputs[i].length > scrollX) {
            char *string = string_to_char_array(inputs[i]);
            mvaddstr(ly, lx, string + scrollX);
            free(string);
            lx += inputs[i].length - scrollX;
        }

        while (lx < COLS)
            mvaddstr(ly, lx++, " ");

        attroff(A_REVERSE);
    }

    move(LINES - lines + y, strlen(labels[y]) + x - scrollX);
}

String *editor_input(Editor *editor, char **labels, int lines) {
    String *inputs = (String *) malloc(sizeof(String) * lines);
    for (int i = 0; i < lines; ++i) {
        inputs[i].length = 0;
        inputs[i].first = NULL;
        inputs[i].last = NULL;
    }

    int x = 0, y = 0;
    bool running = true;
    while (running) {
        editor_display(editor);
        editor_display_input(editor, labels, lines, inputs, y, x);
        refresh();

        int key = getch();

        switch (key) {
        case 17: // Ctrl-Q
        case 27: // Escape
            running = false;
            for (int i = 0; i < lines; ++i)
                string_free(&(inputs[i]));
            inputs = NULL;
            break;

        case 127: // Backspace (macOS)
        case 263: // Backspace (Linux)
            if (x > 0 && string_pop(&(inputs[y]), x - 1) != '\0')
                --x;
            break;

        case 258: // Down arrow
            if (y < lines - 1) ++y;
            x = inputs[y].length;
            break;

        case 259: // Up arrow
            if (y > 0) {
                --y;
                x = inputs[y].length;
            } else x = 0;
            break;

        case 260: // Left arrow
            if (x > 0) --x;
            break;

        case 261: // Right arrow
            if (x < inputs[y].length) ++x;
            break;

        case 330: // Delete
            string_pop(&(inputs[y]), x);
            break;

        case '\n':
            if (y < lines - 1) {
                ++y;
                x = inputs[y].length;
            } else running = false;
            break;

        case 410: // Resize
            break;

        default:
            if ((key >= 32 && key <= 126) || (key >= 128 && key <= 255)) {
                string_insert(&(inputs[y]), (char) key, x);
                ++x;
            } else {
                clear();
                printw("Unknow key (code %d): %c\n", key, key);
                refresh();
                sleep(1);
            }
        }
    }

    return inputs;
}

bool editor_check_saved(Editor *editor) {
    if (editor->saved) return true;

    if (editor->file == NULL) {
        char *labels[] = { " Path to file to save (ESC to cancel, enter to discard changes): " };
        String *inputs = editor_input(editor, labels, 1);

        if (inputs != NULL) {
            if (inputs[0].length > 0) {
                editor->file = string_to_char_array(inputs[0]);
                string_write_file(editor->file, editor->string);
                editor->saved = true;
            }

            string_free(&(inputs[0]));
            free(inputs);
        } else return false;
    } else {
        char *result;
        do {
            char *labels[] = { " Save changes? (Yes/no, ESC to cancel) " };
            String *inputs = editor_input(editor, labels, 1);

            if (inputs != NULL) {
                result = string_to_char_array(inputs[0]);

                string_free(&(inputs[0]));
                free(inputs);
            } else return false;
        } while (strlen(result) != 0 && result[0] != 'Y' && result[0] != 'y' && result[0] != 'N' && result[0] != 'n');

        if (strlen(result) == 0 || result[0] == 'Y' || result[0] == 'y') {
            string_write_file(editor->file, editor->string);
            editor->saved = true;
        }
    }

    return true;
}
