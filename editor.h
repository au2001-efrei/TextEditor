#ifndef EDITOR_H
#define EDITOR_H

#include "string.h"

typedef struct Editor {
	char *file;
	String string;
} Editor;

void editor_display(Editor *editor);

#endif
