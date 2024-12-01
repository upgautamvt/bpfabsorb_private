## Developer Environment,

```cmake
sudo apt install -y git gcc-multilib build-essential gcc g++ cpio fakeroot libncurses5-dev libssl-dev ccache dwarves libelf-dev cmake automake mold libdw-dev libdwarf-dev bpfcc-tools libbpfcc-dev libbpfcc zstd linux-headers-generic libtinfo-dev terminator libstdc++-11-dev libstdc++-12-dev libstdc++-13-dev libstdc++-14-dev bc fping xterm trace-cmd tcpdump flex bison rsync python3-venv ltrace sysdig kmod xdp-tools net-tools iproute2 htop libcap-dev libdisasm-dev binutils-dev unzip pkg-config lsb-release wget curl software-properties-common gnupg zlib1g openssh-client openssh-server strace bpftrace tmux gdb attr busybox vim openssl genisoimage pciutils clang llvm libvirt-daemon-system libvirt-clients qemu-kvm libbpf-dev linux-tools-common
```

```cmake
git clone git@github.com:rosalab/bpfabsorb.gi
git submodule update --init --recursive
cd bpfabsorb/linux # linux root directory
cat /boot/config-$(uname -r) > .config # get the .config file from your running kernel

# do these 4 lines; otherwise make fails. This is required only if you install real linux 
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""

make olddefconfig
#make oldconfig 


fakeroot make -j`nproc` # compiles linux
echo $? # make sure nothing wrong
make headers_install

cd tools/lib/bpf
make 

cd tools/bpf/bpftool
make

# go back to linux root directory
sudo ln -s /usr/include/x86_64-linux-gnu/asm /usr/include/asm

sudo make modules_install
sudo make install
reboot #  you must be able to boot with your compiled custom kernel
