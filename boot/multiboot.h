/*
 * multiboot.h
 * Joel Wolfrath, 2013
 *
 * Multiboot header file
 */	
	
/* Copyright (C) 1999,2003,2007,2008,2009  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../types.h"
     
#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1
     
/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        8192
     
/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
     
/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002
     
/* The bits in the required part of flags field we don't support. */
#define MULTIBOOT_UNSUPPORTED                   0x0000fffc
     
/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000
     
/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004
     
/* Flags set in the 'flags' member of the multiboot header. */
     
/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001
     
/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002
     
/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004
     
/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000
     
/* Flags to be set in the 'flags' member of the multiboot info structure. */
     
/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008
     
/* These next two are mutually exclusive */
     
/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020
     
/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040
     
/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080
     
/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100
     
/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200
     
/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400
     
/* Is there video information? */
#define MULTIBOOT_INFO_VIDEO_INFO               0x00000800
     
#ifndef ASM_FILE
     
struct w_multiboot_header{

	/* Must be MULTIBOOT_MAGIC - see above. */
	w_uint32 magic;
     
	/* Feature flags. */
	w_uint32 flags;
     
	/* The above fields plus this one must equal 0 mod 2^32. */
	w_uint32 checksum;
     
	/* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
	w_uint32 header_addr;
	w_uint32 load_addr;
	w_uint32 load_end_addr;
	w_uint32 bss_end_addr;
	w_uint32 entry_addr;
     
	/* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
	w_uint32 mode_type;
	w_uint32 width;
	w_uint32 height;
	w_uint32 depth;

};
     
/* The symbol table for a.out. */
struct w_multiboot_aout_symbol_table{

	w_uint32 tabsize;
	w_uint32 strsize;
	w_uint32 addr;
	w_uint32 reserved;
};
     
/* The section header table for ELF. */
struct w_multiboot_elf_section_header_table{

	uint num;
	uint size;
	uint addr;
	uint shndx;
};
     
struct w_multiboot_info{

	/* Multiboot info version number */
	w_uint32 flags;
    
	/* Available memory from BIOS */
	w_uint32 mem_lower;
	w_uint32 mem_upper;
     
	/* "root" partition */
	w_uint32 boot_device;
    
	/* Kernel command line */
	w_uint32 cmdline;
     
	/* Boot-Module list */
	w_uint32 mods_count;
	w_uint32 mods_addr;
    
	union{

		struct w_multiboot_aout_symbol_table aout_sym;
		struct w_multiboot_elf_section_header_table elf_sec;
	} u;
     
	/* Memory Mapping buffer */
	w_uint32 mmap_length;
	w_uint32 mmap_addr;
     
	/* Drive Info buffer */
	w_uint32 drives_length;
	w_uint32 drives_addr;
     
	/* ROM configuration table */
	w_uint32 config_table;
     
	/* Boot Loader Name */
	w_uint32 boot_loader_name;
     
	/* APM table */
	w_uint32 apm_table;
     
	/* Video */
	w_uint32 vbe_control_info;
	w_uint32 vbe_mode_info;
	w_uint16 vbe_mode;
	w_uint16 vbe_interface_seg;
	w_uint16 vbe_interface_off;
	w_uint16 vbe_interface_len;
};
     
struct w_multiboot_mod{

	/* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
	w_uint32 mod_start;
	w_uint32 mod_end;
     
	/* Module command line */
	w_uint32 cmdline;
    
	/* padding to take it to 16 bytes (must be zero) */
	w_uint32 pad;
};
     
#endif /* ! ASM_FILE */
     
#endif /* ! MULTIBOOT_HEADER */


