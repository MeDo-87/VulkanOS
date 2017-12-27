#!/bin/bash

# run_bochs.sh
# mounts the correct loopback device, runs bochs, then unmounts.

sudo /sbin/losetup /dev/loop0 ../Running/floppy.img
sudo bochs -f ../Running/bochsrc.txt
sudo /sbin/losetup -d /dev/loop0 
