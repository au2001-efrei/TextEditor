#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "editor.h"
#include "string.h"

void editor_display(Editor *editor) {
    clear();
    char *string = string_to_char_array(editor->string);
    mvaddstr(0, 0, string);
    free(string);
    move(editor->y, editor->x);
    refresh();
}
