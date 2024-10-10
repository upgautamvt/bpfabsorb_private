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
git submodule init

# This will take some time.
git submodule update
```

#### Copy config file to linux folder

``` cp linux-config/.config ./linux ```

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
In an another terminal
```
cd linux
gdb vmlinux
target remote:1234
c
```
then set your breakpoints and debug more


## Adding Ports to QEMU
By default host port 52223 is connected to port 52223 inside the QEMU virtual machine.
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











# Note from Inner unikernel

# 100G Testbed Configuration

## Setup Instructions
Change hostnames and machine names as needed for phobos and deimos.

### Setup netplan
Disable cloudinit network config
```bash
sudo vi /etc/cloud/cloud.cfg.d/90-installer-network.cfg

# --- File should contain this ---

# This is the network config written by 'subiquity'
network: {config: disabled}
```

Add netplan config, make sure `/etc/netplan` has only this config file,
delete the rest.

Make sure to use `192.168.101.1/29` for phobos and `192.168.101.2/29` for
deimos on the 100g interfaces. Use the config in the table above for the
VT net interfaces.

```bash
sudo vi /etc/netplan/50-network.yaml

# --- File should contain this ---

network:
        enp1s0f0np0:
          addresses:
            - 192.168.101.1/29
          mtu: 9000
    version: 2
```

Do apply netplan

```bash
sudo chmod 600 /etc/netplan/01-network-manager-all.yaml
sudo chmod 600 /etc/netplan/50-network.yaml
sudo netplan apply
```


### Set hostnames
```bash
sudo hostnamectl set-hostname phobos.cs.vt.edu
```

Update hosts file to match machine dns names
```bash
sudo vi /etc/hosts
```

Example:
```
127.0.0.1 localhost
127.0.1.1 phobos.cs.vt.edu

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```

### Setup firewall with UFW
```bash
sudo ufw allow from 192.168.101.0/29 to any proto tcp
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw enable
```

### Harden SSH & allow key only
```bash
sudo vi /etc/ssh/sshd_config.d/60-vt.harden.conf

# --- File should contain this ---

PermitRootLogin no
MaxAuthTries 3
LoginGraceTime 20
PasswordAuthentication no
PermitEmptyPasswords no
ChallengeResponseAuthentication no
KerberosAuthentication no
GSSAPIAuthentication no
X11Forwarding no
```

### Install MLNX_EN drivers
Note: Drivers may not support latest Kernel version.
```bash
wget https://www.mellanox.com/downloads/ofed/MLNX_EN-24.04-0.7.0.0/mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64.tgz

tar xaf mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64.tgz

cd mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64 && sudo ./install --add-kernel-support
sudo /etc/init.d/mlnx-en.d restart
```

### Setting up SR-IOV
Heavily updated from [blog post](https://enterprise-support.nvidia.com/s/article/HowTo-Configure-SR-IOV-for-ConnectX-4-ConnectX-5-ConnectX-6-with-KVM-Ethernet#jive_content_id_I_Enable_SRIOV_on_the_Firmware)
```
upgautam@newpc:~/mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64$ lspci | grep Mellanox
0f:00.0 Ethernet controller: Mellanox Technologies MT27700 Family [ConnectX-4]
0f:00.1 Ethernet controller: Mellanox Technologies MT27700 Family [ConnectX-4]


upgautam@newpc:~/mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64$ ip link show
5: enp15s0f0np0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether ec:0d:9a:d6:25:6c brd ff:ff:ff:ff:ff:ff
6: enp15s0f1np1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN mode DEFAULT group default qlen 1000
    link/ether ec:0d:9a:d6:25:6d brd ff:ff:ff:ff:ff:ff

upgautam@newpc:~/mlnx-en-24.04-0.7.0.0-ubuntu24.04-x86_64$ sudo lshw -class network
  *-network UNCLAIMED       
       description: Network controller
       product: RTL8852CE PCIe 802.11ax Wireless Network Controller
       vendor: Realtek Semiconductor Co., Ltd.
       physical id: 0
       bus info: pci@0000:0d:00.0
       version: 01
       width: 64 bits
       clock: 33MHz
       capabilities: pm msi pciexpress cap_list
       configuration: latency=0
       resources: ioport:d000(size=256) memory:fb200000-fb2fffff
  *-network UNCLAIMED
       description: Ethernet controller
       product: RTL8125 2.5GbE Controller
       vendor: Realtek Semiconductor Co., Ltd.
       physical id: 0
       bus info: pci@0000:0e:00.0
       version: 05
       width: 64 bits
       clock: 33MHz
       capabilities: pm msi pciexpress msix vpd cap_list
       configuration: latency=0
       resources: ioport:c000(size=256) memory:fb300000-fb30ffff memory:fb310000-fb313fff
  *-network:0
       description: Ethernet interface
       product: MT27700 Family [ConnectX-4]
       vendor: Mellanox Technologies
       physical id: 0
       bus info: pci@0000:0f:00.0
       logical name: enp15s0f0np0
       version: 00
       serial: ec:0d:9a:d6:25:6c
       capacity: 40Gbit/s
       width: 64 bits
       clock: 33MHz
       capabilities: pciexpress vpd msix pm bus_master cap_list rom ethernet physical 1000bt-fd 10000bt-fd 25000bt-fd 40000bt-fd autonegotiation
```
From above, we see 0f:00.0 is enp15s0f0np0 and that is active.

```
sudo su
mstconfig -d 0f:00.0 query
mstconfig -d 0f:00.0 set NUM_OF_VFS=16
reboot
cat /sys/class/net/enp1s0f0np0/device/sriov_numvfs 
sudo su
echo 16 > /sys/class/net/enp1s0f0np0/device/sriov_numvfs 
```

Now, we can see virtual VFs NICs in lspci. Still we are not done.
We need to config iommu in bios and in boot. We also need to unbind
the VF(s) that we want to use unbind from host.

```
<!--In amd, iommu is enabled by default. If not enable iommu, virtualization in BIOS.
In Intel, enable both in BIOS.
And in /etc/default/grub
        amd: GRUB_CMDLINE_LINUX_DEFAULT="quiet splash amd_iommu=on iommu=pt"
        intel: GRUB_CMDLINE_LINUX_DEFAULT="quiet splash intel_iommu=on iommu=pt"

ALso,
when VFs are there liste in lspci, we first need to unbind from host machine.
        echo 0000:19:00.2 > /sys/bus/pci/drivers/mlx5_core/unbind-->
```

### domain.xml for Virsh
```
<!--In amd, iommu is enabled by default. If not enable iommu, virtualization in BIOS.
In Intel, enable both in BIOS.
And in /etc/default/grub
        amd: GRUB_CMDLINE_LINUX_DEFAULT="quiet splash amd_iommu=on iommu=pt"
        intel: GRUB_CMDLINE_LINUX_DEFAULT="quiet splash intel_iommu=on iommu=pt"

ALso,
when VFs are there liste in lspci, we first need to unbin from host machine.
        echo 0000:19:00.2 > /sys/bus/pci/drivers/mlx5_core/unbind-->

<domain type="kvm">
    <name>ubuntu2404</name>
    <uuid>91953347-42f3-4ca5-bc70-33691aa45c82</uuid>
    <metadata>
        <libosinfo:libosinfo xmlns:libosinfo="http://libosinfo.org/xmlns/libvirt/domain/1.0">
            <libosinfo:os id="http://ubuntu.com/ubuntu/24.04"/>
        </libosinfo:libosinfo>
    </metadata>
    <memory unit="KiB">8388608</memory>
    <currentMemory unit="KiB">8388608</currentMemory>
    <vcpu placement="static">4</vcpu>
    <os>
        <type arch="x86_64" machine="pc-q35-8.2">hvm</type>
        <bootmenu enable="yes"/>
    </os>
    <features>
        <acpi/>
        <apic/>
        <vmport state="off"/>
    </features>
    <cpu mode="host-passthrough" check="none" migratable="on"/>
    <clock offset="utc">
        <timer name="rtc" tickpolicy="catchup"/>
        <timer name="pit" tickpolicy="delay"/>
        <timer name="hpet" present="no"/>
    </clock>
    <on_poweroff>destroy</on_poweroff>
    <on_reboot>restart</on_reboot>
    <on_crash>destroy</on_crash>
    <pm>
        <suspend-to-mem enabled="no"/>
        <suspend-to-disk enabled="no"/>
    </pm>
    <devices>
        <emulator>/usr/bin/qemu-system-x86_64</emulator>
        <disk type="file" device="cdrom">
            <driver name="qemu" type="raw"/>
            <source file="/opt/KVM_VM/iso/ubuntu2404.iso"/>
            <target dev="sda" bus="sata"/>
            <readonly/>
            <boot order="1"/>
            <address type="drive" controller="0" bus="0" target="0" unit="0"/>
        </disk>
        <disk type="file" device="disk">
            <driver name="qemu" type="qcow2" discard="unmap"/>
            <source file="/var/lib/libvirt/images/ubuntu2404.qcow2"/>
            <target dev="sdb" bus="sata"/>
            <boot order="2"/>
            <address type="drive" controller="0" bus="0" target="0" unit="1"/>
        </disk>
        <controller type="usb" index="0" model="qemu-xhci" ports="15">
            <address type="pci" domain="0x0000" bus="0x02" slot="0x00" function="0x0"/>
        </controller>
        <controller type="pci" index="0" model="pcie-root"/>
        <controller type="pci" index="1" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="1" port="0x10"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x0" multifunction="on"/>
        </controller>
        <controller type="pci" index="2" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="2" port="0x11"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x1"/>
        </controller>
        <controller type="pci" index="3" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="3" port="0x12"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x2"/>
        </controller>
        <controller type="pci" index="4" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="4" port="0x13"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x3"/>
        </controller>
        <controller type="pci" index="5" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="5" port="0x14"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x4"/>
        </controller>
        <controller type="pci" index="6" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="6" port="0x15"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x5"/>
        </controller>
        <controller type="pci" index="7" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="7" port="0x16"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x6"/>
        </controller>
        <controller type="pci" index="8" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="8" port="0x17"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x02" function="0x7"/>
        </controller>
        <controller type="pci" index="9" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="9" port="0x18"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x0" multifunction="on"/>
        </controller>
        <controller type="pci" index="10" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="10" port="0x19"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x1"/>
        </controller>
        <controller type="pci" index="11" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="11" port="0x1a"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x2"/>
        </controller>
        <controller type="pci" index="12" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="12" port="0x1b"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x3"/>
        </controller>
        <controller type="pci" index="13" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="13" port="0x1c"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x4"/>
        </controller>
        <controller type="pci" index="14" model="pcie-root-port">
            <model name="pcie-root-port"/>
            <target chassis="14" port="0x1d"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x03" function="0x5"/>
        </controller>
        <controller type="sata" index="0">
            <address type="pci" domain="0x0000" bus="0x00" slot="0x1f" function="0x2"/>
        </controller>
        <controller type="virtio-serial" index="0">
            <address type="pci" domain="0x0000" bus="0x03" slot="0x00" function="0x0"/>
        </controller>
        <controller type="scsi" index="0" model="virtio-scsi">
            <address type="pci" domain="0x0000" bus="0x04" slot="0x00" function="0x0"/>
        </controller>
        <interface type="network">
            <mac address="52:54:00:83:4c:ab"/>
            <source network="default"/>
            <model type="virtio"/>
            <address type="pci" domain="0x0000" bus="0x01" slot="0x00" function="0x0" multifunction="on"/>
        </interface>
        <serial type="pty">
            <target type="isa-serial" port="0">
                <model name="isa-serial"/>
            </target>
        </serial>
        <console type="pty">
            <target type="serial" port="0"/>
        </console>
        <channel type="unix">
            <target type="virtio" name="org.qemu.guest_agent.0"/>
            <address type="virtio-serial" controller="0" bus="0" port="1"/>
        </channel>
        <channel type="spicevmc">
            <target type="virtio" name="com.redhat.spice.0"/>
            <address type="virtio-serial" controller="0" bus="0" port="2"/>
        </channel>
        <input type="tablet" bus="usb">
            <address type="usb" bus="0" port="1"/>
        </input>
        <input type="mouse" bus="ps2"/>
        <input type="keyboard" bus="ps2"/>
        <graphics type="spice" autoport="yes">
            <listen type="address"/>
            <image compression="off"/>
        </graphics>
        <sound model="ich9">
            <address type="pci" domain="0x0000" bus="0x00" slot="0x1b" function="0x0"/>
        </sound>
        <audio id="1" type="spice"/>
        <video>
            <model type="qxl" ram="65536" vram="65536" vgamem="16384" heads="1" primary="yes"/>
            <address type="pci" domain="0x0000" bus="0x00" slot="0x01" function="0x0"/>
        </video>
        <!--        pcie pass-through-->
        <hostdev mode="subsystem" type="pci" managed="yes">
            <source>
                <address domain="0x0000" bus="0x01" slot="0x00" function="0x2"/>
            </source>
            <address type="pci" domain="0x0000" bus="0x07" slot="0x00" function="0x0"/>
        </hostdev>
        <redirdev bus="usb" type="spicevmc">
            <address type="usb" bus="0" port="2"/>
        </redirdev>
        <redirdev bus="usb" type="spicevmc">
            <address type="usb" bus="0" port="3"/>
        </redirdev>
        <watchdog model="itco" action="reset"/>
        <memballoon model="virtio">
            <address type="pci" domain="0x0000" bus="0x05" slot="0x00" function="0x0"/>
        </memballoon>
        <rng model="virtio">
            <backend model="random">/dev/urandom</backend>
            <address type="pci" domain="0x0000" bus="0x06" slot="0x00" function="0x0"/>
        </rng>
    </devices>
</domain>
```

## Project setup directly in host
Clone this repo and its submodules

```c
git clone git@github.com:rosalab/KernelWithBpfPrograms.git (or
https://github.com/rosalab/kernelwithbpfprograms )
cd kernelwithbpfprograms
git submodule update --init --recursive --progress
```

### Compile Linux

```c
cd linux
cp ../q-script/.config .config
make oldconfig
make -j`nproc`
cd -
```

### Install libbpf and bpftool

```c
cd linux/tools/lib/bpf
make -j`nproc`
cd -
```

## Project setup in docker (recommended, you can skip Compile Linux and Install libbpf.. part)
We run QEMU inside a docker container.

```
cd linux
cp ../q-script/.config .config
```

we should just use `sudo make docker`

You also need to enable CONFIG_DYNAMIC_FTRACE, CONFIG_BPF_EVENTS, CONFIG_BPF_SYSCALL, CONFIG_SECURITY, CONFIG_BPF_JIT
For this do `make menuconfig`

Now, `sudo make vmlinux`, which build docker because we eventually use QEMU inside docker anway! You also need to install bpftool and libbpf. In docker you can do using `sudo make bpftool` and `sudo make libbpf`.

Note: search using `/` and navigate using those  numbers.

This is it!

Now, you can do

```c
cd -
sudo make qemu-run // and from another shell, sudo docker qemu-ssh (if needed)
cd -
```

# To remote gdb to QEMU (in host machine)
We have added docker to qemu port mapping
```c
DOCKER_PORT ?= "1234"
-p 127.0.0.1:${DOCKER_PORT}:1234
```

From host, now, we can remote gdb to QEMU
```c
cd -
cd linux
objcopy --only-keep-debug vmlinux kernel.sym
gdb
    (gdb) file ./kernel.sym
    (gdb) target remote localhost:1234 //1234 is our DOCKER_PORT
    (gdb) hbreak trace_call_bpf //this is the function which kprobe handler and tracepoint system use to dispatch bpf programs
    (gdb) c

```

We also need to compile all the bpf programs in bpf-progs directory, and mico in benchmarks/microbenchmarks
```c
cd -
cd bpf-progs
make clean
make
cd -
cd benchmarks/microbenchmarks
gcc -I../../linux/usr/include micro.c -o micro 
```

now, go to QEMU shell
```c
cd bpf-prog //you need to be in bpr-progs because we have ./load.user program that we use to load bpf programs
./load.user bpf_program_file bpf_prog //e.g., ./load.user empty.kern.o empty
```

now, we can run `./micro` that triggers bpf program (or debug)
