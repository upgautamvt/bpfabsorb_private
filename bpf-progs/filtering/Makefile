BPF_CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c

KERN_SRC := $(wildcard *.kern.c)
KERN := $(KERN_SRC:.c=.o)
INCLUDE := -I../../../linux/usr/include -I../../../linux/tools/lib/

USER_SRC := $(wildcard *.user.c)
USER := $(USER_SRC:.c=)
USER_CFLAGS := -I../../../linux/usr/include -I../../../linux/tools/lib/ -L../../../linux/tools/lib/bpf/

all: $(KERN) $(USER)

$(KERN) : %.o : %.c
	$(BPF_CLANG) $(INCLUDE) $(CFLAGS) -o $@ $<

$(USER) : % : %.c
	gcc $(USER_CFLAGS)  $< -lbpf -o $@

.PHONY : clean

clean :
	rm $(KERN) $(USER)








