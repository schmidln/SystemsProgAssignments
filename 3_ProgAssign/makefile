CC = gcc
FLAGS = -Wall -Werror -std=c99
APP = fp_analyze

all: fp_analyzer_f fp_analyzer_d

fp_analyzer_f: fp_analyzer.c main.c
	${CC} ${FLAGS} $^ -o $@

fp_analyzer_d: fp_analyzer.c main.c
	${CC} ${FLAGS} -D DOUBLE $^ -o $@

clean: 
	rm -f fp_analyzer_f fp_analyzer_d
