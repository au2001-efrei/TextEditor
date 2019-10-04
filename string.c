#include <stdio.h>
#include <stdlib.h>

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
