
SOURCES=src
HEADERDIR=$(SOURCES)/headers
CDIR=$(SOURCES)/c
OBJDIR=obj
BINDIR=bin
CFLAGS=-I$(SOURCES)

_DEPS=$(wildcard $(HEADERDIR)/*.h)
DEPS=$(patsubst %,./%,$(_DEPS))

SRCFILES=$(wildcard $(CDIR)/*.c)

_OBJ=$(SRCFILES:c=o)
OBJ=$(patsubst $(CDIR)/%,$(OBJDIR)/%,$(_OBJ))

build: clean $(OBJDIR) $(BINDIR) ppmrw

ppmrw: $(OBJ)
	gcc -o $(BINDIR)/$@ $^ -I.

$(OBJDIR)/%.o: $(CDIR)/%.c $(DEPS)
	gcc -c -o $@ $< -I.

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)