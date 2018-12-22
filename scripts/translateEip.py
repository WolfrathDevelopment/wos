#!/usr/bin/env python

import sys
import os.path

LINKER_FILE = "../linker.out"
if not os.path.isfile(LINKER_FILE):
    print "Couldn't find linker file"
    sys.exit(1)

if len(sys.argv) != 2:
    print "Usage: translateEip.py <EIP address>"
    sys.exit(2)

EIP = sys.argv[1]
EIP_INT = int(EIP, 16)

print "eip: " + EIP

found = False
with open(LINKER_FILE) as f:

    TEXT_HEADER = ".text"

    for line in f:
        cols = line.split()
        if len(cols) > 2 and cols[0] == TEXT_HEADER:
    
            hex_lb = int(cols[1], 16)
            hex_ub = hex_lb + int(cols[2], 16)

            if EIP_INT <= hex_ub and EIP_INT >= hex_lb:
                part = ' '.join(cols[3:])
                if ".o" in part:
                    print part,"offset:",format(EIP_INT - hex_lb, '04X')
                    found = True

if not found:
    print "Could not find",EIP,"in linker map"
