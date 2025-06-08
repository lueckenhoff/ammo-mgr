# FILEHEADER

SHELL = /bin/sh
BUILD_DIR := build
CC = cc
CFLAGS = -Wall -pedantic -g
LD = ${CC}
LDFLAGS = 
RM=/bin/rm -f
TUI_OBJS=\
    ammo_pkg.o \
    brand.o \
    config.o \
    stringdb.o \
    tui_main.o

STRINGDB_TEST_OBJS=stringdb_test.o stringdb.o
.SUFFIXES:
.SUFFIXES: .c .o

# c source
#$(BUILD_DIR)/%.c.o: %.c
#	$(MKDIR_P) $(dir $@)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:	stringdb_test tui

tui:	${TUI_OBJS}
	${LD} $^ -o $@

stringdb_test:	${STRINGDB_TEST_OBJS}
	${LD} $^ -o $@

.PHONY: clean
clean:
	${RM} ${CLI_OBJS} ${STRINGDB_TEST_OBJS}

.PHONY: distclean
distclean:	clean
	${RM} tui stringdb_test
