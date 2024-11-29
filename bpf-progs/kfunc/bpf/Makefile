CLANG ?= clang
LLVM_STRIP ?= llvm-strip
BPF_TARGET := bpf

CFLAGS := -O2 -g -target $(BPF_TARGET) -Wall -Werror -I/usr/include

KERN_C_FILES := $(wildcard *.kern.c)
OBJECTS := $(KERN_C_FILES:.kern.c=.o)

all: $(OBJECTS)

%.kern.o: %.kern.c
	$(CLANG) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
