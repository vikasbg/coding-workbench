all: tooltip

tooltip: main.c
	gcc -g -O0 -o $@ main.c

check: all
	./tooltip | grep "Hello from .*tooltip"
	@echo "*** ALL TESTS PASSED ***"

install:
	install -d $(prefix)/build
	install -m 0755 tooltip $(prefix)/build

uninstall:
	rm -f $(prefix)/build/tooltip
	rmdir $(prefix)/build > /dev/null 2>&1

clean:
	rm -f tooltip

.PHONY: all clean check install uninstall