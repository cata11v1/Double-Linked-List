all:build
build: double_list.c
	gcc -o double_list double_list.c -g -Wall -Wextra -std=c99 -lm

clean:build
	rm -rf *.o double_list
