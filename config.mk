PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CFLAGS   = -std=c99 -Wall -pedantic -O2
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE
LDFLAGS  = -s
