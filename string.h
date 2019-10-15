#ifndef STRING_H
#define STRING_H

typedef struct Character {
    char data;
    struct Character *prev;
    struct Character *next;
} Character;

typedef struct String {
    int length;
    Character *first;
    Character *last;
} String;

String string_read_file(char *path);
void string_write_file(char *path, String string);

char *string_to_char_array(String string);
char *string_get_line(String string, int line);

int string_get_line_count(String string);
int string_get_offset(String string, int y);
int string_get_line_length(String string, int y);

void string_concatenate(String *string, char *string2, int position);
void string_insert(String *string, char c, int position);
char string_pop(String *string, int position);

String string_copy(String string);

#endif
