APP = matrix_mult
CC = gcc
FLAGS = -Wall -Werror -std=gnu99

all: $(APP)

$(APP): $(APP).c main.c
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f $(APP)

