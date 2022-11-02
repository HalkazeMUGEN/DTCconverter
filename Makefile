CC := x86_64-w64-mingw32-gcc.exe
SRCDIR := ./src
OUTDIR := ./build
INCDIR := ./include
LIBDIR := ./lib
PROGRAM := $(OUTDIR)/DTCconverter.exe
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(SRCS))
DEPENDS := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.d,$(SRCS))
HEADERS := $(wildcard $(INCDIR)/*.h)
LIBS := -lregex
LDFLAGS := -L$(LIBDIR) $(LIBS) -Wl,--nxcompat,--no-seh,--disable-forceinteg,--subsystem,console
WARNINGS := -Wall -Wextra
OPTIMIZERS :=  -O2 -fomit-frame-pointer -ftree-vectorize
TARGETS := -mavx -mavx2 -mmmx -msse -msse2 -msse2avx -msse3 -msse4 -msse4.1 -msse4.2 -mssse3 -mfpmath=sse
CFLAGS := $(WARNINGS) $(TARGETS) $(OPTIMIZERS) -g -DNDEBUG
CFLAGS_DEBUG := $(WARNINGS) $(TARGETS) -O0 -g -DDEBUG
LDFLAGS_DEBUG := -L$(LIBDIR) $(LIBS) -Wl,--nxcompat,--no-seh,--disable-forceinteg,--disable-dynamicbase,--subsystem,console

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(SRCS) $(HEADERS)
	cd $(OUTDIR) && $(MAKE) "CC=$(CC)" "CFLAGS=$(CFLAGS)"
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.PHONY: debug
debug:
	$(MAKE) "CFLAGS=$(CFLAGS_DEBUG)" "LDFLAGS=$(LDFLAGS_DEBUG)"

.PHONY: clean
clean:
	$(RM) -f $(OBJS) $(DEPENDS) $(PROGRAM)
