BPF_CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c

KERNEL_DIR = ../../../linux

KERN_SRC := $(wildcard *.kern.c)
KERN := $(KERN_SRC:.c=.o)
INCLUDE := -I$(KERNEL_DIR)/usr/include -I$(KERNEL_DIR)/tools/lib/

USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
USER_CFLAGS := -I$(KERNEL_DIR)/usr/include -I$(KERNEL_DIR)/tools/lib/ -L$(KERNEL_DIR)/tools/lib/bpf/

all: $(KERN) $(USER)

$(KERN) : %.o : %.c
	$(BPF_CLANG) $(INCLUDE) $(BPF_CLANG_CFLAGS) -o $@ $<

$(USER) : % : %.c
	gcc $(USER_CFLAGS)  $< -lbpf -o $@

.PHONY : clean

clean :
	rm $(KERN) $(USER)