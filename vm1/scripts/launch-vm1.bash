#!/bin/bash

qemu-system-x86_64  \
  -enable-kvm \
  -smp 4 \
  -cpu host \
  -m 8G \
  -nographic \
  -device virtio-net-pci,netdev=net0 \
  -netdev user,id=net0,hostfwd=tcp::2222-:22 \
  -drive if=virtio,format=qcow2,file=../images/noble-server-cloudimg-amd64-vm1.img \
  -drive if=virtio,media=cdrom,file=../seeds/vm1-seed.iso \
  -device vfio-pci,host=0000:01:00.2 
