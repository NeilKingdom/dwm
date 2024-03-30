# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

SRCS := $(wildcard $(SRC_DIR)/*.c)
DEPS := $(wildcard $(INC_DIR)/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: options dwm

options:
	@echo dwm build options:
	@echo "CCFLAGS  = $(CCFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $< -c -o $@ $(CCFLAGS) $(LDFLAGS)

dwm: $(OBJS)
	$(CC) $^ -o $@ $(CCFLAGS) $(LDFLAGS)

clean:
	rm -f dwm $(OBJ_DIR)/*

dist: clean
	mkdir -p dwm-$(VERSION)
	cp -r LICENSE Makefile README.md config.mk \
		$(SRC_DIR) $(INC_DIR) $(OBJ_DIR) res dwm-$(VERSION)
	tar -cf dwm-$(VERSION).tar dwm-$(VERSION)
	gzip dwm-$(VERSION).tar
	rm -rf dwm-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f dwm $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dwm
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < res/doc/dwm.1 > $(DESTDIR)$(MANPREFIX)/man1/dwm.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/dwm.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwm $(DESTDIR)$(MANPREFIX)/man1/dwm.1

.PHONY: all options clean dist install uninstall
