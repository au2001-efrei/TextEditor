#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string.h"

String string_read_file(char *path) {
    String string;
    string.length = 0;
    string.first = NULL;
    string.last = NULL;

    FILE *file = fopen(path, "r");
    if (!file) return string;

    Character *current = NULL;

    char c = fgetc(file);
    if (!feof(file)) {
        current = (Character *) malloc(sizeof(Character));
        current->data = c;
        current->next = NULL;
        current->prev = NULL;
        string.first = current;
        ++string.length;

        c = fgetc(file);
        while (!feof(file)) {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = c;
            current->next = NULL;
            ++string.length;

            c = fgetc(file);
        }

        string.last = current;
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

String string_from_char_array(char *string) {
    String result;
    result.length = strlen(string);
    result.first = NULL;
    result.last = NULL;

    if (result.length > 0) {
        Character *current = (Character *) malloc(sizeof(Character));
        current->data = string[0];
        current->next = NULL;
        current->prev = NULL;
        result.first = current;

        for (int i = 1; i < result.length; ++i) {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = string[i];
            current->next = NULL;
        }

        result.last = current;
    }

    return result;
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
    while (current != NULL && current->data != '\n') {
        current = current->next;
        ++length;
    }

    return length;
}

bool string_starts_with(Character *string, char *prefix) {
    Character *current = string;
    int length = strlen(prefix);
    for (int i = 0; i < length; ++i) {
        if (current == NULL || current->data != prefix[i])
            return false;

        current = current->next;
    }

    return true;
}

int string_search(String string, char *search, int position) {
    Character *current = string.first;
    for (int i = 0; i < position && current != NULL; ++i)
        current = current->next;

    int offset = 0;
    while (current != NULL) {
        if (string_starts_with(current, search))
            return offset;

        ++offset;
        current = current->next;
    }

    return -1;
}

void string_insert(String *string, char c, int position) {
    if (position <= 0) {
        Character *temp = (Character *) malloc(sizeof(Character));
        temp->data = c;
        temp->prev = NULL;
        temp->next = string->first;
        if (string->first != NULL) string->first->prev = temp;
        string->first = temp;
        ++string->length;
        return;
    }

    int i = 1;
    Character *current = string->first;
    while (i < position && current != NULL) {
        current = current->next;
        ++i;
    }

    if (current == NULL) return;

    Character *temp = (Character *) malloc(sizeof(Character));
    temp->data = c;
    temp->prev = current;
    temp->next = current->next;
    if (temp->next != NULL) temp->next->prev = temp;
    current->next = temp;

    ++string->length;
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

    Character *next = current != NULL ? current->next : string->first;

    int i = 0, length = strlen(string2);
    if (length > 0 && current == NULL) {
        current = (Character *) malloc(sizeof(Character));
        current->data = string2[0];
        current->next = NULL;
        current->prev = NULL;
        string->first = current;
        ++i;
    }

    for (; i < length; ++i) {
        current->next = (Character *) malloc(sizeof(Character));
        current->next->prev = current;
        current = current->next;
        current->data = string2[i];
        current->next = NULL;
    }

    string->length += length;

    if (next == NULL) string->last = current;
    else if (current != NULL) {
        next->prev = current;
        current->next = next;
    }
}

void string_concatenate_string(String *string, String string2, int position) {
    Character *current = NULL;

    if (string->first != NULL && position > 0) {
        current = string->first;

        int i = 1;
        while (i < position && current->next != NULL) {
            current = current->next;
            ++i;
        }
    }

    Character *next = current != NULL ? current->next : string->first;

    string2 = string_copy(string2);
    if (current == NULL) {
        string->first = string2.first;
        current = string->first;
    } else current->next = string2.first;

    while (current->next != NULL)
        current = current->next;

    if (next == NULL) string->last = current;
    else if (current != NULL) {
        next->prev = current;
        current->next = next;
    }
}

char string_pop(String *string, int position) {
    if (string->first == NULL) return '\0';

    if (position <= 0) {
        char c = string->first->data;

        if (string->first->next != NULL) {
            string->first = string->first->next;
            free(string->first->prev);
            string->first->prev = NULL;
        } else {
            free(string->first);
            string->first = NULL;
            string->last = NULL;
        }

        --string->length;

        return c;
    }

    Character *current = string->first;

    int i = 1;
    while (i < position && current->next != NULL) {
        current = current->next;
        ++i;
    }

    if (current->next == NULL) return '\0';

    char c = current->next->data;
    Character *next = current->next->next;

    if (next != NULL)
        next->prev = current;
    else string->last = current;

    free(current->next);
    current->next = next;

    --string->length;

    return c;
}

char *string_delete(String *string, int position, int length) {
    char *result = (char *) malloc(sizeof(char) * length);

    for (int i = 0; i < length; ++i)
        result[i] = string_pop(string, position);

    return result;
}

int string_replace(String *string, char *search, char *replacement) {
    int result = 0;

    Character *current = string->first;
    int position = 0;
    while (current != NULL) {
        if (string_starts_with(current, search)) {
            current = current->prev;

            free(string_delete(string, position, strlen(search)));
            string_concatenate(string, replacement, position);
            ++result;

            if (current == NULL) current = string->first;
            else current = current->next;

            int length = strlen(replacement);
            for (int i = 0; i < length; ++i) {
                current = current->next;
                ++position;
            }
        } else {
            ++position;
            current = current->next;
        }
    }

    return result;
}

String string_copy(String string) {
    String copy;
    copy.length = string.length;
    copy.first = NULL;
    copy.last = NULL;

    Character *current = NULL, *currentRead = string.first;
    if (currentRead != NULL) {
        current = (Character *) malloc(sizeof(Character));
        current->data = currentRead->data;
        current->next = NULL;
        current->prev = NULL;
        copy.first = current;
        currentRead = currentRead->next;

        while (currentRead != NULL) {
            current->next = (Character *) malloc(sizeof(Character));
            current->next->prev = current;
            current = current->next;
            current->data = currentRead->data;
            current->next = NULL;
            currentRead = currentRead->next;
        }

        copy.last = current;
    }

    return copy;
}

void string_free(String *string) {
    string->length = 0;

    Character *current = string->first;
    if (current == NULL) return;

    while (current->next != NULL) {
        current = current->next;
        free(current->prev);
    }

    free(current);

    string->first = NULL;
    string->last = NULL;
}
