package = tooltip
version = 1.0
tarname = $(package)
distdir = $(tarname)-$(version)

prefix = $(shell pwd)
# prefix = /usr/local
export prefix 
# export make prefix available in the src branch Makefile as well.
# Otherwise it will only available in the current folder.

all clean check install uninstall tooltip: # All these targets are inside src branch Makefile
	cd src && $(MAKE) $@

dist: $(distdir).tar.gz

$(distdir).tar.gz: $(distdir)
	tar chof - $(distdir) | gzip -9 -c > $@
	rm -rf $(distdir)

$(distdir): FORCE
	mkdir -p $(distdir)/src
	cp Makefile $(distdir)
	cp src/Makefile src/main.c $(distdir)/src

distcheck: $(distdir).tar.gz
	gzip -cd $(distdir).tar.gz | tar xvf -
	cd $(distdir) && $(MAKE) all
	cd $(distdir) && $(MAKE) check
	cd $(distdir) && $(MAKE) clean
	rm -rf $(distdir)
	@echo "*** Package $(distdir).tar.gz is ready for distribution."

distclean: FORCE
FORCE:
	rm -f $(distdir).tar.gz
	rm -rf $(distdir)

.PHONY: FORCE all clean check dist distcheck install uninstall