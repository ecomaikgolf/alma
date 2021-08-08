BUILDDIR=build
FILESYSTEMDIR=$(BUILDDIR)/filesystem

all: run 

run: 
	make -C $(BUILDDIR)
debug: 
	make -C $(BUILDDIR) debug
