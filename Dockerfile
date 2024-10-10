FROM ubuntu:22.04 AS linux-builder


ENV PATH=/linux/tools/lib/bpf/:$PATH \
    LD_LIBRARY_PATH=/linux/tools/lib/bpf:$LD_LIBRARY_PATH \
    LINUX=/linux

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y
RUN DEBIAN_FRONTEND=noninteractive apt-get install --fix-missing -y git build-essential gcc g++ fakeroot libncurses5-dev libssl-dev ccache dwarves libelf-dev \
 cmake mold \
 libdw-dev libdwarf-dev \
 bpfcc-tools libbpfcc-dev libbpfcc \
 linux-headers-generic \
 libtinfo-dev terminator \
 libstdc++-11-dev libstdc++-12-dev \
 bc clang llvm \
 flex bison \
 rsync \
 libcap-dev libdisasm-dev binutils-dev unzip \
 pkg-config lsb-release wget software-properties-common gnupg zlib1g llvm \
 qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon xterm attr busybox openssh-server \
 iputils-ping kmod
