#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

char *skip_whitespace(char *in);
char *skip_untill_whitespace(char *in);
char *skip_untill_next_word(char *in);
char *skip_untill_x_char(char *in, char x);
char *get_substring(char *ptr, size_t size);
void get_substring_dest(char *ptr, size_t size, char *dest);
char *skip_to_nextline(char *in);

#endif
