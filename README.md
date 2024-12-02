## Developer Environment,

```cmake
sudo apt install -y git gcc-multilib build-essential gcc g++ cpio fakeroot libncurses5-dev libssl-dev ccache dwarves libelf-dev cmake automake mold libdw-dev libdwarf-dev bpfcc-tools libbpfcc-dev libbpfcc zstd linux-headers-generic libtinfo-dev terminator libstdc++-11-dev libstdc++-12-dev libstdc++-13-dev libstdc++-14-dev bc fping xterm trace-cmd tcpdump flex bison rsync python3-venv ltrace sysdig kmod xdp-tools net-tools iproute2 htop libcap-dev libdisasm-dev binutils-dev unzip pkg-config lsb-release wget curl software-properties-common gnupg zlib1g openssh-client openssh-server strace bpftrace tmux gdb attr busybox vim openssl genisoimage pciutils clang llvm libvirt-daemon-system libvirt-clients qemu-kvm libbpf-dev linux-tools-common
```

```cmake
git clone git@github.com:rosalab/bpfabsorb.gi
git submodule update --init --recursive
cd bpfabsorb/linux # linux root directory

# Copy the current running kernel config to the kernel source directory
cp /boot/config-$(uname -r) .config

# do these 4 lines; otherwise make fails. This is required only if you install real linux 
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""


# Update the .config file for the new kernel version
make olddefconfig

# Optional: Review and change kernel configuration if needed
make menuconfig

# Build the kernel image (using multiple cores to speed up the process)
make -j$(nproc)

# Build the kernel modules
make modules

# Install the kernel headers
sudo make headers_install

# Install the kernel modules
sudo make modules_install

# Install the kernel image
sudo make install

# Update the bootloader (if necessary)
sudo update-grub   # On Debian/Ubuntu-based systems

# Reboot into the new kernel
sudo reboot



# Docker setup
#### Build linux

```
make vmlinux
```

#### Run Qemu
```
make qemu-run
```

#### If you want to ssh into the qemu
```
make qemu-ssh
```

#### If you want to enter the docker container where qemu is running
```
make enter-docker
```

#### If you want to debug the kernel using gdb
```
make qemu-run-gdb
```
In a terminal
```
cd linux
gdb vmlinux
target remote:1234
c
```
then set your breakpoints and debug more


## Adding Ports to QEMU
By default, host port 52223 is connected to port 52223 inside the QEMU virtual machine.
If you need to be able to connect to more than one port (or a specific port) on your custom kernel from the host, you will have to add new rules.
The needed rules are in `q-script/yifei-q` and in the Makefile.

### Makefile modifications
You must add a line that maps a host port to a Docker port.
In the Makefile you must add a line
```-p 127.0.0.1:HOST_PORT:DOCKER_PORT```
This will map the host port to the docker port.

### q-script modifications
You must modify the q-script to connect the DOCKER_PORT to a QEMU_PORT.
In the q-script you must append a new rule.
Find the line that starts with `"net += -netdev user..."`.
Then at the end of the line add the text ```"hostfwd=tcp::DOCKER_PORT-:QEMU_PORT"```


**Note**: The idea is we can develop/debug in qemu (i.e., make qemu-run), but when we need real VM, we use vm<X> folders above.
Because inside vm<X> we can install mellanox driver. We can't install mellanox driver in our qemu. Also, for cases like
when we need multiple NIcs (including mellanox VFIO nic), we can do that in real vm<X. 



Git branching strategies
For main repo, and for all other submodules, we like to have same named branch. Checkout to same branch

Make a bash script: ./checkout_all_same_branch.sh
```cmake
# Check out the branch in the main repository
git fetch origin                                     # Fetch the latest changes for the main repo
if git show-ref --quiet refs/heads/dev/upg/kernel_absorb; then
    git checkout dev/upg/kernel_absorb              # If branch exists, switch to it
else
    git checkout -b dev/upg/kernel_absorb origin/dev/upg/kernel_absorb  # Create and switch to it
fi

# Check out the same branch in each submodule
for submodule in linux vm1 vm2; do
    cd $submodule
    git fetch origin                                   # Fetch the latest changes from the remote
    if git show-ref --quiet refs/heads/dev/upg/kernel_absorb; then
        git checkout dev/upg/kernel_absorb           # If branch exists, switch to it
    else
        git checkout -b dev/upg/kernel_absorb origin/dev/upg/kernel_absorb  # Create and switch to it
    fi
    cd ..
done
```
And run: ./checkout_all_same_branch.sh
You can test
```cmake
rosa@sriov-vm2:~/bpfabsorb$ git branch
* dev/upg/kernel_absorb
  main
  rosa@sriov-vm2:~/bpfabsorb$ cd linux/
  rosa@sriov-vm2:~/bpfabsorb/linux$ git branch
* dev/upg/kernel_absorb
  master
  rosa@sriov-vm2:~/bpfabsorb/linux$ cd ../vm1
  rosa@sriov-vm2:~/bpfabsorb/vm1$ git branch
* dev/upg/kernel_absorb
  master
  rosa@sriov-vm2:~/bpfabsorb/vm1$ cd ../vm2
  rosa@sriov-vm2:~/bpfabsorb/vm2$ git branch
* dev/upg/kernel_absorb
  master
```

