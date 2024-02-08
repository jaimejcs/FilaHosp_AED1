#Makefile para gerar o binário executável main.exe

all: main

main: hospital.c hospital.h main.c
	gcc -o hospital.o hospital.c -c -W -Wall
	gcc -o main hospital.o main.c

clean:
	del /f hospital.o