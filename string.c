#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

String string_read_file(char *path) {
    String string;
    string.length = 0;
    string.first = NULL;
    string.last = NULL;

    FILE *file = fopen(path, "r");

    if (!file) {
        return string;
    }

    Character *current = NULL;

    char c = fgetc(file);
    while (!feof(file)) {
        if (current == NULL) {
            current = (Character *) malloc(sizeof(Character));
            current->data = c;
            current->next = NULL;
            current->prev = NULL;
        } else {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = c;
            current->next = NULL;
        }

        if (string.first == NULL) string.first = current;
        string.last = current;
        ++string.length;

        c = fgetc(file);
    }

    fclose(file);

    return string;
}

void string_write_file(char *path, String string) {
    Character *current = string.first;

    FILE *file = fopen(path, "w");

    while (current != NULL) {
        fputc(current->data, file);

        current = current->next;
    }

    fclose(file);
}

char *string_to_char_array(String string) {
    char *array = (char *) malloc(sizeof(char) * (string.length + 1));

    int i = 0;
    Character *current = string.first;
    while (current != NULL) {
        array[i] = current->data;

        current = current->next;
        ++i;
    }

    array[i] = '\0';

    return array;
}

char *string_get_line(String string, int y) {
    int line = 0;
    Character *current = string.first;
    while (current != NULL && line < y) {
        if (current->data == '\n') ++line;

        current = current->next;
    }

    int length = 0;
    Character *current2 = current;
    while (current2 != NULL && current2->data != '\n') {
        current2 = current2->next;
        ++length;
    }

    char *array = (char *) malloc(sizeof(char) * (length + 2));

    int i = 0;
    while (current != NULL && current->data != '\n') {
        array[i] = current->data;

        current = current->next;
        ++i;
    }

    array[length] = '\n';
    array[length + 1] = '\0';

    return array;
}

int string_get_line_count(String string) {
    int count = 1;
    Character *current = string.first;
    while (current != NULL) {
        if (current->data == '\n') ++count;

        current = current->next;
    }

    return count;
}

int string_get_offset(String string, int y) {
    int line = 0, offset = 0;
    Character *current = string.first;
    while (current != NULL && line < y) {
        if (current->data == '\n') ++line;

        current = current->next;
        ++offset;
    }

    return offset;
}

int string_get_line_length(String string, int y) {
    int line = 0;
    Character *current = string.first;
    while (current != NULL && line < y) {
        if (current->data == '\n') ++line;

        current = current->next;
    }

    int length = 0;
    Character *current2 = current;
    while (current2 != NULL && current2->data != '\n') {
        current2 = current2->next;
        ++length;
    }

    return length;
}

void string_append(String *string, char c) {
    if (string->last != NULL) {
        string->last->next = (Character *) malloc(sizeof(Character));
        string->last->next->prev = string->last;
        string->last = string->last->next;
        string->last->data = c;
        string->last->next = NULL;

        ++string->length;
    } else {
        string->last = (Character *) malloc(sizeof(Character));
        string->last->data = c;
        string->last->next = NULL;
        string->last->prev = NULL;
        string->first = string->last;

        string->length = 1;
    }
}

void string_concatenate(String *string, char *string2, int position) {
    Character *current = NULL;

    if (string->first != NULL && position > 0) {
        current = string->first;

        int i = 1;
        while (i < position && current->next != NULL) {
            current = current->next;
            ++i;
        }
    }

    Character *next = position > 0 ? current->next : string->first;

    int length = strlen(string2);
    for (int i = 0; i < length; ++i) {
        if (current == NULL) {
            current = (Character *) malloc(sizeof(Character));
            current->data = string2[i];
            current->next = NULL;
            current->prev = NULL;
            string->first = current;
        } else {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = string2[i];
            current->next = NULL;
        }

        ++string->length;
    }

    if (next == NULL) string->last = current;
    else if (current != NULL) {
        next->prev = current;
        current->next = next;
    }
}

char string_pop_last(String *string) {
    if (string->last == NULL) return '\0';

    char c = string->last->data;

    if (string->last->prev != NULL) {
        string->last = string->last->prev;
        free(string->last->next);
        string->last->next = NULL;

        --string->length;
    } else {
        free(string->last);
        string->last = NULL;
        string->first = NULL;

        string->length = 0;
    }

    return c;
}

String string_copy(String string) {
    String copy;

    copy.length = string.length;
    copy.first = NULL;
    copy.last = NULL;

    Character *current = NULL, *currentRead = string.first;
    while (currentRead != NULL) {
        if (current == NULL) {
            current = (Character *) malloc(sizeof(Character));
            current->data = currentRead->data;
            current->next = NULL;
            current->prev = NULL;
            copy.first = current;
        } else {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = currentRead->data;
            current->next = NULL;
        }

        copy.last = current;
        currentRead = currentRead->next;
    }

    return copy;
}
