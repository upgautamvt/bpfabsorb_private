# Define source files and their corresponding object files for BPF programs
KERN_SRC := $(wildcard *.kern.c)
FILES := $(KERN_SRC:.c=.o)

# Define user-space source files and their corresponding executables
USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)

# BPF and Clang settings
BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c
INCLUDE := -I../../linux/usr/include/ -I../../linux/tools/lib/
USER-CFLAGS := -I../../linux/usr/include -I../../linux/tools/lib/

OUTPUT_DIR := output

# Default target to build all object files and user programs
all: $(FILES) $(USER)

# Rule to compile user-space programs
$(USER) : % : %.c
	$(BPF-CLANG) $(USER-CFLAGS) $< -lbpf -o $@
	
# Rule to compile BPF programs into object files
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(CFLAGS) -o $@ $<


# Generate BPF skeletons for all .kern.o files
# Generate BPF skeletons and corresponding .skel.c files
skel: $(FILES)
	@mkdir -p $(OUTPUT_DIR)
	for obj in $(FILES); do \
		name=$$(basename $$obj .kern.o); \
		bpftool gen skeleton $$obj name $$name | tee $(OUTPUT_DIR)/$$name.skel.h > /dev/null 2>&1; \
		cp template.c $(OUTPUT_DIR)/$$name.skel.c; \
		sed -i "s/%s/$$name/g" $(OUTPUT_DIR)/$$name.skel.c; \
	done

# Generate Makefile inside output directory
output_makefile:
	@cp finalMakefile $(OUTPUT_DIR)/Makefile

# Clean rule to remove generated files
.PHONY: clean

clean:
	rm -f $(FILES) $(USER) *.skel.h
	rm -rf $(OUTPUT_DIR)
