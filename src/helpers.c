#include <ctype.h>
#include <string.h>

// Compares string a and string b, ignoring case.
// Returns the difference between the ascii values
// of the first non matching characters, or the
// difference between their lengths, if applicable.
int strcmp_nocase(const char *a, const char *b) {
	int alen = strlen(a);
	int blen = strlen(b);
	int dif = 0;
	for (int i=0; i<alen && i<blen; i++) {
		dif = tolower(a[i]) - towlower(b[i]);
		if (dif != 0) {
			return dif;
		}
	}
	if (alen != blen) {
		return alen - blen;
	}
	return dif;
}

int min(int a, int b) {
	if (a<b) {
		return a;
	} else {
		return b;
	}
}

int max(int a, int b) {
	if (a>b) {
		return a;
	} else {
		return b;
	}
}
