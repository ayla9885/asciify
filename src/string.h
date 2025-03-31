#ifndef STRING_H
#define STRING_H

struct {
	char *chars;
	int length;
	int capacity;
} typedef String;

String *string_new();
String *string_from(const char*);
void string_free(String *);

void string_grow(String*, int);
void string_print(const String*);
void string_println(const String*);
void string_push(String*, const char);
void string_pop(String*);
void string_concat(String*, const String*);
void string_concat_lit(String*, const char*);

#endif
