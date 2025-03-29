BUILDDIR = build
SOURCEDIR = src

asciify: $(SOURCEDIR)/main.c
	mkdir $(BUILDDIR) -p
	gcc -std=gnu99 -Wall -o build/asciify_dev src/main.c -g -Og

release: $(SOURCEDIR)/main.c
	mkdir $(BUILDDIR) -p
	gcc -std=gnu99 -Wall -Werror -o build/asciify src/main.c -O2
