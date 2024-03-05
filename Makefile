#Makefile para gerar o binário executável main.exe

all: main

main: object 
	gcc -I include -o main obj/hospital.o main.c -W -Wall

object: src/hospital.c
	gcc -I include -c src/hospital.c -o obj/hospital.o -W -Wall
