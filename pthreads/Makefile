TARGETS = demo1 demo2

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -Werror -pthread

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@
clean:
	rm -f $(TARGETS)

#to do on command line, gcc filename -std=c99 -pthread -o executable name
