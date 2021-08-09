CC=clang
CFLAGS=`pkg-config --cflags raylib`
#CLFAGS+=-I./vendor/sqlite3/
LDFLAGS=`pkg-config --libs raylib`

SRCDIR=src
BUILDDIR=build
VENDORDIR=vendor
OUT=clova

OBJS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
OBJS+=$(BUILDDIR)/sqlite3.o

all: $(BUILDDIR)/$(OUT)

run: $(BUILDDIR)/$(OUT)
	./$(BUILDDIR)/$(OUT)

clean:
	rm -rf $(BUILDDIR)

.PHONY: clean

$(BUILDDIR)/$(OUT): $(BUILDDIR) $(OBJS)
	$(CC) $(OBJS) -o $(BUILDDIR)/$(OUT) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CLFAGS)

$(BUILDDIR)/sqlite3.o: $(VENDORDIR)/sqlite3/sqlite3.c
	$(CC) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

