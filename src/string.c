#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "string.h"

String *string_new() {
	String *str = malloc(sizeof(*str));
	str->length = 0;
	str->capacity = 0;
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
	fwrite(str->chars, sizeof *str->chars, str->length, stdout);
}

void string_println(const String *str) {
	string_print(str);
	putchar('\n');
}

void string_fwrite(const String *str, FILE *stream) {
	fwrite(str->chars, sizeof *str->chars, str->length, stream);
}

void string_push(String *str, const char c) {
	str->length++;
	if (str->length > str->capacity) {
		string_grow(str, 1);
	}
	str->chars[str->length-1] = c;
}

void string_start_color(String *str, int r, int g, int b) {
	string_concat_lit(str, "\033[38;2;");
	char tmp[33];
	snprintf(tmp, sizeof tmp, "%i", r);
	string_concat_lit(str, tmp);
	string_push(str, ';');
	snprintf(tmp, sizeof tmp, "%i", g);
	string_concat_lit(str, tmp);
	string_push(str, ';');
	snprintf(tmp, sizeof tmp, "%i", b);
	string_concat_lit(str, tmp);
	string_concat_lit(str, "m");
}

void string_end_color(String *str) {
	string_concat_lit(str, "\033[0m");
}

void string_pop(String *str) {
	str->length--;
}

void string_concat(String *dest, const String *src) {

}

void string_concat_lit(String* dest, const char* src) {
	int src_len = strlen(src);
	int base = dest->length;
	dest->length += src_len;
	if (dest->length > dest->capacity) {
		string_grow(dest, src_len);
	}
	strncpy(dest->chars+base, src, src_len);
}
