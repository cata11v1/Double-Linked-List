all:build
build: tema1.c
	gcc -o tema1 tema1.c -g -Wall -Wextra -std=c99 -lm

clean:build
	rm -rf *.o tema1
