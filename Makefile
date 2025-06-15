# FILEHEADER

SHELL = /bin/sh
BUILD_DIR := build
CC = cc
CFLAGS = -Wall -pedantic -g
LD = ${CC}
LDFLAGS = 
RM=/bin/rm -f
CLI_OBJS=\
    ammo_pkg.o \
    brand.o \
    bullet.o \
    caliber.o \
    config.o \
    item.o \
    stringdb.o \
    cli_main.o

TUI_OBJS=\
    ammo_pkg.o \
    brand.o \
    bullet.o \
    caliber.o \
    config.o \
    item.o \
    stringdb.o \
    tui_main.o

STRINGDB_TEST_OBJS=stringdb_test.o stringdb.o
.SUFFIXES:
.SUFFIXES: .c .o

# c source
#$(BUILD_DIR)/%.c.o: %.c
#	$(MKDIR_P) $(dir $@)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:	stringdb_test tui cli

cli:	${CLI_OBJS}
	${LD} $^ -o $@

tui:	${TUI_OBJS}
	${LD} $^ -o $@

stringdb_test:	${STRINGDB_TEST_OBJS}
	${LD} $^ -o $@

.PHONY: clean
clean:
	${RM} ${CLI_OBJS} ${TUI_OBJS} ${STRINGDB_TEST_OBJS}

.PHONY: distclean
distclean:	clean
	${RM} cli tui stringdb_test
