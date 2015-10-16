CFLAGS=-g -Wall -Wextra

SRC=\
	source/item.c \
	source/test.c \

default:
	$(CC) $(CFLAGS) -o meow $(SRC)

clean:
	- rm -f meow
