.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

all: xtest mksocket

.o:
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

xtest.o: arg.h

check: xtest mksocket
	./test

install: xtest
	mkdir -p -- "$(DESTDIR)$(PREFIX)/bin"
	mkdir -p -- "$(DESTDIR)$(MANPREFIX)/man1"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/share/licenses/xtest"
	cp -- xtest "$(DESTDIR)$(PREFIX)/bin/"
	cp -- xtest.1 "$(DESTDIR)$(MANPREFIX)/man1/"
	cp -- LICENSE "$(DESTDIR)$(PREFIX)/share/licenses/xtest/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/bin/xtest"
	-rm -f -- "$(DESTDIR)$(MANPREFIX)/man1/xtest.1"
	-rm -rf -- "$(DESTDIR)$(PREFIX)/share/licenses/xtest"

clean:
	-rm -rf -- xtest mksocket *.o .testdir

.SUFFIXES: .o .c.o

.PHONY: all install uninstall clean
