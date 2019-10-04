#include <stdio.h>
#include <ncurses.h>

#include "editor.h"
#include "string.h"

void editor_display(Editor *editor) {
	clear();
	addstr(string_to_char_array(editor->string));
	refresh();
}
