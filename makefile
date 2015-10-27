CFLAGS   +=-O0 -gdwarf-4 -Wall -Wextra -pthread
CXXFLAGS += $(CFLAGS) -DRAPIDXML_NO_EXCEPTIONS -std=gnu++11 -Irapidxml -DBOOST_ALL_DYN_LINK
#debian needs libboost-all-dev
LDFLAGS  := -lboost_system -lboost_filesystem -lboost_log

SRC=\
	source/rend/image.cpp\
	source/rend/imageatlas.cpp\
	source/rend/resourcemanager.cpp\
	source/character.c\
	source/object.c\

default: meow

meow: $(SRC)
	$(CC) $(CFLAGS) -c $(filter %.c, $(SRC))
	$(CXX) $(CXXFLAGS) -c $(filter %.cpp, $(SRC))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o meow *.o

clean:
	- rm -f meow *.o
