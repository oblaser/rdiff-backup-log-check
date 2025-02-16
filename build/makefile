
# author        Oliver Blaser
# date          15.02.2025
# copyright     GPL-3.0 - Copyright (c) 2025 Oliver Blaser

# make && ./rdiff-backup-log-check /path/to/backup-dir ; echo $?


CC = g++
LINK = g++

CFLAGS = -c -I.. --std=c++17 -O3 -pedantic -Wall -Werror=return-type -Werror=switch -Werror=reorder -Werror=format
LFLAGS = -O3

OBJS = main.o
EXE = rdiff-backup-log-check

BUILDDATE = $(shell date +"%Y-%m-%d-%H-%M")



ifeq ($(prefix),)
    prefix = /usr/local
endif

BIN_PATH=$(prefix)/bin
HEADER_PATH=$(prefix)/include
LIB_PATH=$(prefix)/lib



$(EXE): $(OBJS)
	$(LINK) $(LFLAGS) -o $(EXE) $(OBJS)

main.o: ../main.cpp ../project.h
	$(CC) $(CFLAGS) ../main.cpp

all: $(EXE)
	

clean:
	rm $(OBJS)
	rm $(EXE)

install: $(EXE)
	install -m 755 $(EXE) $(BIN_PATH)
