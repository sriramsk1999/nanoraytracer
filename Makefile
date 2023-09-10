##
# nanoraytracer
#
# @file

CC = g++

CFLAGS = -g
INCFLAGS = -I./include/glm-0.9.7.1 -I./include/
LDFLAGS =

RM = /bin/rm -f
all: nanoraytracer
nanoraytracer: main.o Transform.o readfile.o variables.h readfile.h Transform.h
	$(CC) $(CFLAGS) -o nanoraytracer main.o Transform.o readfile.o $(INCFLAGS) $(LDFLAGS)
main.o: main.cpp Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
readfile.o: readfile.cpp readfile.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
clean:
	$(RM) *.o nanoraytracer *.png

# end
