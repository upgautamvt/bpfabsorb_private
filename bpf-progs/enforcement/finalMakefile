## Define directories
LIB_DIR := $(shell realpath ../../../linux/tools/lib)
INCLUDE_DIR := $(shell realpath ../../../linux/usr/include)
BPF_LIB_DIR := $(shell realpath ../../../linux/tools/lib/bpf)

## Define source files
SKEL_SRCS := $(wildcard *.skel.c)
SKEL_BINS := $(SKEL_SRCS:.skel.c=Final)

## Compiler options
CC := clang
CFLAGS := -g -w
LIBS := -lbpf

## Targets
all: $(SKEL_BINS)

## Rule to compile any .skel.c file into a binary
%Final: %.skel.c
	$(CC) $(CFLAGS) -I"$(INCLUDE_DIR)" -I"$(LIB_DIR)" -L"$(BPF_LIB_DIR)" $< $(LIBS) -o $@

## Target to run all generated binaries
run: $(SKEL_BINS)
	@for bin in $(SKEL_BINS); do \
		./$bin; \
	done

## Clean up generated files
clean:
	rm -f $(SKEL_BINS)