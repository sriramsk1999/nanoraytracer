##
# nanoraytracer
#
# @file

CC = g++

CFLAGS = -g
INCFLAGS = -I./include/glm-0.9.7.1 -I./include/
LDFLAGS = -L./lib/ -lfreeimage

RM = /bin/rm -f
all: nanoraytracer
nanoraytracer: main.o Transform.o Scene.o Raytracer.o readfile.o variables.h readfile.h Transform.h
	$(CC) $(CFLAGS) -o nanoraytracer main.o Transform.o Scene.o Raytracer.o readfile.o $(INCFLAGS) $(LDFLAGS)
main.o: main.cpp Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
readfile.o: readfile.cpp readfile.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
Scene.o: Scene.cpp Scene.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Scene.cpp
Raytracer.o: Raytracer.cpp Raytracer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Raytracer.cpp
clean:
	$(RM) *.o nanoraytracer *.png

# end
