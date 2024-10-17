# Linux Development Environment
This repository contains *one* workflow for building and modifying the Linux kernel.
It consists of two main components.
The first is a docker container that contains all the requirements to build the Linux kernel, as well
as the requirements to run QEMU.
The second is a QEMU  script that boots a virtual machine running a custom version of the Linux kernel.
Using these together allows you to easily make and test changes to the Linux kernel without needing to
manage all the packages locally.

***This repository is cloned and modified from rosalab/(unknown)-kernel***

#### Build Docker Container

``` make docker ```

#### Update git submodules
The `linux` directory contains a forked linux kernel source tree as a git submodule. The below commands help you to update it.

```sh
git submodule update --init --recursive
```

#### Copy config file to linux folder

``` cp q-script/.config ./linux ```

or

```
cd linux
make defconfig
cd ../
```

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

