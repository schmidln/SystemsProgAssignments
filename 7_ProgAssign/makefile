APP = mem_alloc
CC = gcc
FLAGS = -Wall -Werror -std=gnu99

${APP}: $(APP).c main.c
	${CC} ${FLAGS} $^ -o $@

test: $(APP).c test_main.c
	gcc $(FLAGS) $^ -o $@

clean: 
	rm -f mem_alloc test
