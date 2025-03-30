BUILD_DIR = build
SOURCE_DIR = src
vpath %.c = $(SOURCE_DIR)
vpath %.h = $(SOURCE_DIR)

CC = gcc
CFLAGS = -Wall -g

asciify: main.o filter.o image.o helpers.o 
	mkdir -p $(BUILD_DIR)
	$(CC) $(addprefix $(BUILD_DIR)/,$^) -o $(BUILD_DIR)/$@ $(CFLAGS)

%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -o $(BUILD_DIR)/$@ $<

main.o: filter.h image.h helpers.h
filter.o: image.h helpers.h
image.o: helpers.h
helpers.o: 

clean:
	rm $(BUILD_DIR)/*.o
	rm $(BUILD_DIR)/asciify*
