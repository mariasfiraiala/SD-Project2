# Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

CC=gcc
CFLAGS=-I -Wall -Wextra -std=c99 -g

build: 
	$(CC) -o main hashtable.c library_op.c main.c op_utils.c users_op.c $(CFLAGS)

clean:
	rm -f *.o main