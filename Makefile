CC=gcc
CFLAGS=-Wall -Werror -O2
NAME=-o ptree
INCLUDE=-I include/
SRC=src/*

.PHONY: all ptree
all:
	${CC} ${CFLAGS} ${INCLUDE} ${NAME} ${SRC}

ptree:
	./ptree

