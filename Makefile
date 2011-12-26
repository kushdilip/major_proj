#Makefile for my project

CC = gcc
SOURCES = major_proj.c jpeg_fun.c
OBJECTS = major_proj.o jpeg_fun.o
HEADERS = jpeg_fun.h
OUT = ./bin/major_proj

major_proj: $(OBJECTS)
	$(CC) -o major_proj $(OBJECTS) -ljpeg
	strip $(OUT)
major_proj.o: $(HEADERS)

jpeg_fun.o: jpeg_fun.h

clean:
	rm $(OBJECTS) major_proj



