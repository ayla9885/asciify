asciify: src/main.c
	gcc -std=gnu99 -Wall -o build/asciify_dev src/main.c -g

release: src/main.c
	gcc -std=gnu99 -Wall -Werror -o build/asciify src/main.c -O2
