#ifndef EDITOR_H
#define EDITOR_H

#include "string.h"

typedef struct Editor {
    char *file;
    String string;
    int x, y;
} Editor;

void editor_display(Editor *editor);

void editor_display_input(Editor *editor, char **labels, int lines, String *input, int y, int x);
String *editor_input(Editor *editor, char **labels, int lines);

#endif
