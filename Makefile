PREFIX = /usr
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share
MANDIR = $(DATADIR)/man
MAN1DIR = $(MANDIR)/man1
LICENSEDIR = $(DATADIR)/licenses

PKGNAME = xtest
COMMAND = xtest

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE
CFLAGS   = -std=c99 -Wall -pedantic

all: xtest

xtest: xtest.o

.o:
	$(CC) $(LDFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

install: install-base install-doc

install-base: install-cmd install-copyright

install-cmd: xtest
	mkdir -p -- "$(DESTDIR)$(BINDIR)"
	cp -f -- xtest "$(DESTDIR)$(BINDIR)/$(COMMAND)"
	chmod 755 -- "$(DESTDIR)$(BINDIR)/$(COMMAND)"

install-copyright: install-license

install-license:
	mkdir -p -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"
	cp -f -- LICENSE "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/LICENSE"
	chmod 644 -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/LICENSE"

install-doc: install-man

install-man: install-man1

install-man1:
	mkdir -p -- "$(DESTDIR)$(MAN1DIR)"
	cp -f -- xtest.1 "$(DESTDIR)$(MAN1DIR)/$(COMMAND).1"
	chmod 644 -- "$(DESTDIR)$(MAN1DIR)/$(COMMAND).1"

uninstall:
	rm -- "$(DESTDIR)$(MAN1DIR)/$(COMMAND).1"
	rm -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/LICENSE"
	rm -- "$(DESTDIR)$(BINDIR)/$(COMMAND)"
	rmdir -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"

.PHONY: all install install-base install-cmd install-copyright install-license \
        install-doc install-man install-man1 uninstall
