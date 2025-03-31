#ifndef STRING_H
#define STRING_H

#include <stdio.h>

struct {
	char *chars;
	int length;
	int capacity;
} typedef String;

String *string_new();
String *string_from(const char *lit);
void string_free(String *str);
void string_grow(String *str, int);

void string_print(const String *str);
void string_println(const String *str);
void string_fwrite(const String *str, FILE *stream);

void string_push(String *str, const char c);
void string_start_color(String *str, int r, int g, int b);
void string_end_color(String *str);
void string_pop(String *str);

void string_concat(String *dest, const String *src);
void string_concat_lit(String *dest, const char *src);

#endif
