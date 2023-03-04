./USERID=123456789

CC = g++
CFLAGS := $(CFLAGS) -Wall -Wextra --debug -Icommon_src
LDFLAGS := $(LDFLAGS) -Lcommon_src

SRC_SERVER = $(wildcard server_src/*.cpp)
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
DEP_SERVER = $(SRC_SERVER:.cpp=.d)

SRC_CLIENT = $(wildcard client_src/*.cpp)
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)
DEP_CLIENT = $(SRC_CLIENT:.cpp=.d)

SRC_COMMON = $(wildcard common_src/*.cpp)
OBJ_COMMON = $(SRC_COMMON:.cpp=.o)
DEP_COMMON = $(SRC_COMMON:.cpp=.d)

.PHONY: build clean dist tarball

build: server client

server: $(OBJ_SERVER) $(OBJ_COMMON)
	$(CC) -o $@ $(LDFLAGS) $^

client: $(OBJ_CLIENT) $(OBJ_COMMON)
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.cpp Makefile
	$(CC) -c -o $@ $(CFLAGS) -MMD -MP $<

-include $(DEP_SERVER)
-include $(DEP_CLIENT)
-include $(DEP_COMMON)

clean:
	rm -f $(OBJ_SERVER) $(DEP_SERVER) $(OBJ_CLIENT) $(DEP_CLIENT) $(OBJ_COMMON) $(DEP_SERVER) \
		./server ./client
	rm -rf *.tar.gz *.file

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .
