SOURCES := $(wildcard *.kern.c)
FILES := $(KERN_SRC:.c=.o)
BPF-CLANG := clang
BPF_CLANG_CFLAGS := -target bpf -g -Wall -O2 -c
INCLUDE := -I../../linux/usr/include/ -I../../linux/tools/lib/

all: $(FILES)
	
$(FILES) : %.o : %.c
	$(BPF-CLANG) $(INCLUDE) $(CFLAGS) -o $@ $<

.PHONY : clean

map-fine-clean:
	rm $(FILES)

clean :
	rm $(FILES)