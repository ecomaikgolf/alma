BUILDDIR=build
FILESYSTEMDIR=$(BUILDDIR)/filesystem

all: dev-run 

dev-run: 
	make -C $(BUILDDIR)
