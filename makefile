CFLAGS=-O0 -gdwarf-4 -Wall -Wextra -fdiagnostics-color
	
#	source/test.c \

SRC=\
	source/object.c \
	source/character.c

default:
	$(CC) $(CFLAGS) -o meow $(SRC)

clean:
	- rm -f meow
