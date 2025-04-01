BUILD_DIR = build
SOURCE_DIR = src
vpath %.c = $(SOURCE_DIR)
vpath %.h = $(SOURCE_DIR)

CC = gcc
CFLAGS = -Wall -g -std=gnu99

asciify: main.o filter.o image.o helpers.o string.o
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $(BUILD_DIR)/$@ $(CFLAGS) -lm

# %.o: %.c
# 	mkdir -p $(BUILD_DIR)
# 	$(CC) -c $(CFLAGS) -o $(BUILD_DIR)/$@ $<

main.o: filter.h image.h helpers.h string.h
filter.o: image.h helpers.h string.h
image.o: helpers.h
helpers.o: 
string.o:

clean:
	rm ./*.o
	rm $(BUILD_DIR)/asciify*
