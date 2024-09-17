# dwm version
VERSION = 6.4

# Customize below to fit your system

# Paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Xinerama, comment if you don't want it
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# Freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2

# Includes and libs
INCS = -I$(X11INC) -I$(FREETYPEINC) -Iinclude
LIBS = -L$(X11LIB) $(XINERAMALIBS) $(FREETYPELIBS) -lX11 -lX11-xcb -lxcb -lxcb-res -lmpdclient

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\" $(XINERAMAFLAGS)
CCFLAGS = -std=c99 -Wall -Wno-deprecated-declarations -Os $(INCS) $(CPPFLAGS)
LDFLAGS = $(LIBS)

CC = gcc
