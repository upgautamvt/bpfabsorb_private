KERNEL_DIR = ../../../linux

# Define the current directory (where the module source file is located)
PWD := $(shell pwd)

# Enable BTF generation
KBUILD_CFLAGS += -g -O2

# Name of the module (tc_ingress_clsact_filter)
obj-m += tc_ingress_clsact_filter.o
obj-m += bpf_strstr.o
obj-m += xdp_kfunc_module.o


# Makefile rules to build the module
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

# Clean up the build artifacts
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean