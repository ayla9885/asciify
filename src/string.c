#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "string.h"

String *string_new() {
	String *str = malloc(sizeof(*str));
	str->length = 0;
	str->chars = malloc(sizeof(*str->chars));
	return str;
}

String *string_from(const char *lit) {
	String *str = malloc(sizeof(*str));
	int len = strlen(lit);
	str->length = len;
	str->capacity = len;
	str->chars = malloc((sizeof *str->chars) * str->capacity);
	strncpy(str->chars, lit, str->capacity);
	return str;
}

void string_free(String *str) {
	free(str->chars);
	str->chars = NULL;
	str->length = 0;
	str->capacity = 0;
	free(str);
}

void string_grow(String *str, int n) {
		str->capacity += n;
		str->chars = realloc(str->chars, (str->capacity * sizeof *str->chars));
}

void string_print(const String *str) {
	for (int i=0; i<str->length; i++) {
		putchar(str->chars[i]);
	}
}

void string_println(const String *str) {
	string_print(str);
	putchar('\n');
}

void string_push(String *str, const char c) {
	str->length++;
	if (str->length > str->capacity) {
		string_grow(str, 1);
	}
	str->chars[str->length-1] = c;
}

void string_pop(String *str) {
	str->length--;
}

void string_concat(String *strA, const String *strB) {

}

void string_concat_lit(String* str, const char* lit) {
	int lit_len = strlen(lit);
	int base = str->length;
	str->length += lit_len;
	if (str->length > str->capacity) {
		string_grow(str, lit_len);
	}
	strncpy(str->chars+base, lit, lit_len);
}
