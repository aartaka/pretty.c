.PHONY: indent clean check install

CFLAGS ?= -lm

indent: test.c pretty.h
	-indent -linux --procnames-start-lines $^

clean:
	-rm test

check: test
	./test

install:
	$(INSTALL) pretty.h $(DESTDIR)$(PREFIX)/include/
