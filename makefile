
SOURCES=src
HEADERDIR=$(SOURCES)/headers
CDIR=$(SOURCES)/c
OBJDIR=obj
BINDIR=bin
CFLAGS=-I$(SOURCES) -pthread -O3

_DEPS=$(wildcard $(HEADERDIR)/*.h)
DEPS=$(patsubst %,./%,$(_DEPS))

SRCFILES=$(wildcard $(CDIR)/*.c)

_OBJ=$(SRCFILES:c=o)
OBJ=$(patsubst $(CDIR)/%,$(OBJDIR)/%,$(_OBJ))

build: clean $(OBJDIR) $(BINDIR) raytrace

raytrace: $(OBJ)
	gcc -o $(BINDIR)/$@ $^ $(CFLAGS)

$(OBJDIR)/%.o: $(CDIR)/%.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)