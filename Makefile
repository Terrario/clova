CC=clang
CFLAGS=`pkg-config --cflags raylib`
LDFLAGS=`pkg-config --libs raylib`

SRCDIR=src
BUILDDIR=build
OUT=clova

OBJS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

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

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

