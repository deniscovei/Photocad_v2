# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra 
#-std=c99

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

build: $(OBJS)
	$(CC) $(CFLAGS) -o image_editor $(OBJS) -lm

pack:
	zip -FSr 312CA_CoveiDenis_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean
