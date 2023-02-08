USERID=123456789

default: build

# TODO: Add header & D file flags
build: server.cpp client.cpp
	g++ -Wall -Wextra -o server server.cpp
	g++ -Wall -Wextra -o client client.cpp

clean:
	rm -rf *.o server client *.tar.gz

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .