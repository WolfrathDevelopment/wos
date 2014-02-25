#!/bin/bash

# run_bochs.sh
# mounts the correct loopback device, runs bochs, then unmounts.

sudo /sbin/losetup /dev/loop0 floppy.img
sudo ./../../Downloads/bochs-20140121/bochs -q -f bochsrc.txt 
sudo /sbin/losetup -d /dev/loop0 
