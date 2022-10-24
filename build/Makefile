CC := gcc
SRCDIR := ../src
INCDIR := ../include
OUTDIR := .
PROGRAM := $(OUTDIR)/DTCconverter.exe
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(SRCS))
DEPENDS := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.d,$(SRCS))
HEADERS := $(wildcard $(INCDIR)/*.h)

.PHONY: build
build: $(OBJS)

%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INCDIR)

%.d: $(SRCDIR)/%.c $(HEADERS)
	cpp -MM $< -MF $@ -I$(INCDIR)

-include $(DEPENDS)
