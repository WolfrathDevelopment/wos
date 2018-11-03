#!/bin/bash
sudo losetup /dev/loop255 floppy.img
sudo mount /dev/loop255 /mnt
sudo cp ../kernel /mnt/kernel
sudo umount /dev/loop255
sudo losetup -d /dev/loop255 
