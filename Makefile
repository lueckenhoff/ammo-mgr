# FILEHEADER

SHELL = /bin/sh
BUILD_DIR := build
CC = cc
CFLAGS = -Wall -g
LD = ${CC}
LDFLAGS = 
CLI_OBJS=cli_main.o ammo_pkg.o config.o stringdb.o
STRINGDB_TEST_OBJS=stringdb_test.o stringdb.o
.SUFFIXES:
.SUFFIXES: .c .o

# c source
#$(BUILD_DIR)/%.c.o: %.c
#	$(MKDIR_P) $(dir $@)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:	stringdb_test cli

cli:	${CLI_OBJS}
	${LD} $^ -o $@

stringdb_test:	${STRINGDB_TEST_OBJS}
	${LD} $^ -o $@

.PHONY: clean
clean:
	${RM} ${CLI_OBJS} ${STRINGDB_TEST_OBJS}

.PHONY: distclean
distclean:	clean
	${RM} cli
