// This file is a part of std libraries
// https://github.com/xcatalyst/std
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//
// MIT License
// Copyright (c) Çağan Korkmaz <cagankorkmaz35@gmail.com>
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdint.h>
#include <stdbool.h>

/* Note most of the comments are taken directly from the specsification page,
 * either wikipedia or MicroSoft's own website to avoid any mistakes */

/*==================================================
 *                                                 *
 *                                                 *
 *                ELF Format Begin                 *
 *                                                 *
 *                                                 *
 *================================================*/

typedef struct {
	unsigned char	ident[16];	/* Magic number and other info */
	uint16_t	type;		/* Object file type */
	uint16_t	isa;		/* Architecture */
	uint32_t	version;	/* Object file version */
	uint32_t	entry;		/* Entry point virtual address */
	uint32_t	phoff;		/* Program header table file offset */
	uint32_t	shoff;		/* Section header table file offset */
	uint32_t	flags;		/* Processor-specific flags */
	uint16_t	ehsize;	/* ELF header size in bytes */
	uint16_t	phentsize;	/* Program header table entry size */
	uint16_t	phnum;		/* Program header table entry count */
	uint16_t	shentsize;	/* Section header table entry size */
	uint16_t	shnum;		/* Section header table entry count */
	uint16_t	shstrndx;	/* Section header string table index */
} elf32_header;

typedef struct {
	unsigned char	ident[16];	/* Magic number and other info */
	uint16_t	type;		/* Object file type */
	uint16_t	isa;		/* Architecture */
	uint32_t	version;	/* Object file version */
	uint64_t	entry;		/* Entry point virtual address */
	uint64_t	phoff;		/* Program header table file offset */
	uint64_t	shoff;		/* Section header table file offset */
	uint32_t	flags;		/* Processor-specific flags */
	uint16_t	ehsize;	/* ELF header size in bytes */
	uint16_t	phentsize;	/* Program header table entry size */
	uint16_t	phnum;		/* Program header table entry count */
	uint16_t	shentsize;	/* Section header table entry size */
	uint16_t	shnum;		/* Section header table entry count */
	uint16_t	shstrndx;	/* Section header string table index */
} elf64_header;

static struct {
	unsigned char	ident[16];
	char*	type[10];
	char*	isa[79];
} elf_map_str = {

	/* File type */
	.type[0] = "Unknwon type",
	.type[1] = "Relocatable file",
	.type[2] = "Executable file",
	.type[3] = "Shared object",
	.type[4] = "Core file",
	.type[5] = "", /* Reserved include range. Operating system specific */
	.type[6] = "", /* Reserved include range. Operating system specific */
	.type[7] = "", /* Reserved include range. Operating system specific */
	.type[8] = "", /* Reserved include range. Operating system specific */
	.type[9] = "", /* Reserved include range. Operating system specific */

	/* ISA */
	.isa[0] = "No specific instruction set",
	.isa[1] = "AT&T WE 32100",
	.isa[2] = "SPARC",
	.isa[3] = "x86",
	.isa[4] = "Motorola 68000 (M68k)",
	.isa[5] = "Motorola 88000 (M88k)",
	.isa[6] = "Intel MCU",
	.isa[7] = "Intel 80860",
	.isa[8] = "MIPS",
	.isa[9] = "IBM System/370",
	.isa[10] = "MIPS RS3000 Little-endian",
	.isa[11] = "future use ",
	.isa[12] = "Hewlett-Packard PA-RISC",
	.isa[13] = "future use",
	.isa[14] = "Intel 80960",
	.isa[15] = "PowerPC",
	.isa[16] = "PowerPC (64-bit)",
	.isa[17] = "S390, including S390x",
	.isa[18] = "IBM SPU/SPC",
	.isa[19] = "future use",
	.isa[20] = "NEC V800",
	.isa[21] = "Fujitsu FR20",
	.isa[22] = "TRW RH-32",
	.isa[23] = "Motorola RCE",
	.isa[24] = "Arm (up to Armv7/AArch32)",
	.isa[25] = "Digital Alpha",
	.isa[26] = "SuperH",
	.isa[27] = "SPARC Version 9",
	.isa[28] = "Siemens TriCore embedded processor",
	.isa[29] = "Argonaut RISC Core",
	.isa[30] = "Hitachi H8/300",
	.isa[31] = "Hitachi H8/300H",
	.isa[32] = "Hitachi H8S",
	.isa[33] = "Hitachi H8/500",
	.isa[34] = "IA-64",
	.isa[35] = "Stanford MIPS-X",
	.isa[36] = "Motorola ColdFire",
	.isa[37] = "Motorola M68HC12",
	.isa[38] = "Fujitsu MMA Multimedia Accelerator",
	.isa[39] = "Siemens PCP",
	.isa[40] = "Sony nCPU embedded RISC processor",
	.isa[41] = "Denso NDR1 microprocessor",
	.isa[42] = "Motorola Star*Core processor",
	.isa[43] = "Toyota ME16 processor",
	.isa[44] = "STMicroelectronics ST100 processor",
	.isa[45] = "Advanced Logic Corp. TinyJ embedded processor family",
	.isa[46] = "AMD x86-64",
	.isa[47] = "Sony DSP Processor",
	.isa[48] = "Digital Equipment Corp. PDP-10",
	.isa[49] = "Digital Equipment Corp. PDP-11",
	.isa[50] = "Siemens FX66 microcontroller",
	.isa[51] = "STMicroelectronics ST9+ 8/16 bit microcontroller",
	.isa[52] = "STMicroelectronics ST7 8-bit microcontroller",
	.isa[53] = "Motorola MC68HC16 Microcontroller",
	.isa[54] = "Motorola MC68HC11 Microcontroller",
	.isa[55] = "Motorola MC68HC08 Microcontroller",
	.isa[56] = "Motorola MC68HC05 Microcontroller",
	.isa[57] = "Silicon Graphics SVx",
	.isa[58] = "STMicroelectronics ST19 8-bit microcontroller",
	.isa[59] = "Digital VAX",
	.isa[60] = "Axis Communications 32-bit embedded processor",
	.isa[61] = "Infineon Technologies 32-bit embedded processor",
	.isa[62] = "Element 14 64-bit DSP Processor",
	.isa[63] = "LSI Logic 16-bit DSP Processor",
	.isa[64] = "future use",
	.isa[65] = "TMS320C6000 Family",
	.isa[66] = "future use",
	.isa[67] = "MCST Elbrus e2k",
	.isa[68] = "future use",
	.isa[69] = "Arm 64-bits (Armv8/AArch64)",
	.isa[70] = "future use",
	.isa[71] = "Zilog Z80",
	.isa[72] = "future use",
	.isa[73] = "RISC-V",
	.isa[74] = "future use",
	.isa[75] = "Berkeley Packet Filter",
	.isa[76] = "future use",
	.isa[77] = "WDC 65C816",
	.isa[78] = "or future use",
};

enum elf_index{
	ELF_INDEX_ARCH = 4,
	ELF_INDEX_ENDIAN = 5,
	ELF_INDEX_VERSION = 6,
	ELF_INDEX_ABI = 7,
	ELF_INDEX_ABIVER = 8,
};

enum elf_arch {
	ELF_X32,
	ELF_X64,
};

enum elf_endian{
	ELF_LITTLE_ENDIAN,
	ELF_BIG_ENDIAN,
};
enum elf_ident_ver {
	ELF_IDENT_VER_1,
};

enum elf_abi {
	ELF_SYSTEMV = 0,
	FILE_FORMAT_HPUX,
	ELF_NETBSD,
	ELF_LINUX,
	ELF_GNUHURD,
	ELF_SOLARIS,
	ELF_AIX,
	ELF_IRIX,
	ELF_FREEBSD,
	ELF_TRU64,
	ELF_NOVELLMODESTO,
	ELF_OPENBSD,
	ELF_OPENVMS,
	ELF_NONSTOPKERNEL,
	ELF_AROS,
	ELF_FENIXOS,
	ELF_NUXI,
	ELF_STRATUS_OPENVOS
};

enum elf_abiver {
	ELF_UNKNOWN = 0x00000000,
	ELF_VER1 = 0x01000000,
	ELF_VER2 = 0x02000000,
	ELF_VER3 = 0x03000000,
	ELF_VER4 = 0x04000000,
	ELF_VER5 = 0x05000000,
};

enum elf_type {
	ELF_TYPE_NONE = 0,		/* Unknown */
	ELF_TYPE_REL = 1,		/* Relocatable file */
	ELF_TYPE_EXEC = 2,		/* Executable file */
	ELF_TYPE_DYN = 3,		/* Shared object */
	ELF_TYPE_CORE = 4,		/* Core file */
	ELF_TYPE_NUM = 5,		/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_LOOS = 0xfe00,	/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_HIOS = 0xfeff,	/* Reserved inclusive range. Operating system specific */
	ELF_TYPE_LOPROC = 0xff00,	/* Reserved inclusive range. Processor specific */
	ELF_TYPE_HIPROC = 0xffff,	/* Reserved inclusive range. Processor specific */
};

enum elf_isa {
	ELF_ISA_UNKNOWN              = 0x00, /* No specific instruction set */
	ELF_ISA_ATAT32100            = 0x01, /* AT&T WE 32100 */
	ELF_ISA_SPARC                = 0x02, /* SPARC */
	ELF_ISA_X86                  = 0x03, /* x86 */
	ELF_ISA_MOTOROLA_680000      = 0x04, /* Motorola 68000 (M68k) */
	ELF_ISA_MOTOROLA_88000       = 0x05, /* Motorola 88000 (M88k) */
	ELF_ISA_INTEL_MCU            = 0x06, /* Intel MCU */
	ELF_ISA_INTEL_80860          = 0x07, /* Intel 80860 */
	ELF_ISA_MIPS                 = 0x08, /* MIPS */
	ELF_ISA_IBM_SYSTEM_370       = 0x09, /* IBM System/370 */
	ELF_ISA_MIPS_RS3000_LE       = 0x0A, /* MIPS RS3000 Little-endian */
	/* ELF_ISA 0x0B - 0x0E: Reserved for future use */
	ELF_ISA_HP_PA_RISC           = 0x0F, /* Hewlett-Packard PA-RISC */
	/* ELF_ISA 0x10 - 0x12: Reserved for future use */
	ELF_ISA_INTEL_80960          = 0x13, /* Intel 80960 */
	ELF_ISA_POWERPC              = 0x14, /* PowerPC */
	ELF_ISA_POWERPC_64           = 0x15, /* PowerPC (64-bit) */
	ELF_ISA_S390                 = 0x16, /* S390, including S390x */
	ELF_ISA_IBM_SPU_SPC          = 0x17, /* IBM SPU/SPC */
	/* ELF_ISA 0x18 - 0x23: Reserved for future use */
	ELF_ISA_NEC_V800             = 0x24, /* NEC V800 */
	ELF_ISA_FUJITSU_FR20         = 0x25, /* Fujitsu FR20 */
	ELF_ISA_TRW_RH32             = 0x26, /* TRW RH-32 */
	ELF_ISA_MOTOROLA_RCE         = 0x27, /* Motorola RCE */
	ELF_ISA_ARM                  = 0x28, /* Arm (up to Armv7/AArch32) */
	ELF_ISA_DIGITAL_ALPHA        = 0x29, /* Digital Alpha */
	ELF_ISA_SUPERH               = 0x2A, /* SuperH */
	ELF_ISA_SPARC_V9             = 0x2B, /* SPARC Version 9 */
	ELF_ISA_SIEMENS_TRICORE      = 0x2C, /* Siemens TriCore embedded processor */
	ELF_ISA_ARGONAUT_RISC_CORE   = 0x2D, /* Argonaut RISC Core */
	ELF_ISA_HITACHI_H8300        = 0x2E, /* Hitachi H8/300 */
	ELF_ISA_HITACHI_H8300H       = 0x2F, /* Hitachi H8/300H */
	ELF_ISA_HITACHI_H8300S       = 0x30, /* Hitachi H8S */
	ELF_ISA_HITACHI_H8500        = 0x31, /* Hitachi H8/500 */
	ELF_ISA_IA_64                = 0x32, /* IA-64 */
	ELF_ISA_STANFORD_MIPS_X      = 0x33, /* Stanford MIPS-X */
	ELF_ISA_MOTOROLA_COLDFIRE    = 0x34, /* Motorola ColdFire */
	ELF_ISA_MOTOROLA_M68HC12     = 0x35, /* Motorola M68HC12 */
	ELF_ISA_FUJITSU_MMA          = 0x36, /* Fujitsu MMA Multimedia Accelerator */
	ELF_ISA_SIEMENS_PCP          = 0x37, /* Siemens PCP */
	ELF_ISA_SONY_NCPU            = 0x38, /* Sony nCPU embedded RISC processor */
	ELF_ISA_DENSO_NDR1           = 0x39, /* Denso NDR1 microprocessor */
	ELF_ISA_MOTOROLA_STARCORE    = 0x3A, /* Motorola Star*Core processor */
	ELF_ISA_TOYOTA_ME16          = 0x3B, /* Toyota ME16 processor */
	ELF_ISA_STM_ST100            = 0x3C, /* STMicroelectronics ST100 processor */
	ELF_ISA_ALC_TINYJ            = 0x3D, /* Advanced Logic Corp. TinyJ embedded processor family */
	ELF_ISA_AMD_X86_64           = 0x3E, /* AMD x86-64 */
	ELF_ISA_SONY_DSP             = 0x3F, /* Sony DSP Processor */
	ELF_ISA_DIGITAL_PDP_10       = 0x40, /* Digital Equipment Corp. PDP-10 */
	ELF_ISA_DIGITAL_PDP_11       = 0x41, /* Digital Equipment Corp. PDP-11 */
	ELF_ISA_SIEMENS_FX66         = 0x42, /* Siemens FX66 microcontroller */
	ELF_ISA_STM_ST9P             = 0x43, /* STMicroelectronics ST9+ 8/16 bit microcontroller */
	ELF_ISA_STM_ST7              = 0x44, /* STMicroelectronics ST7 8-bit microcontroller */
	ELF_ISA_MOTOROLA_MC68HC16    = 0x45, /* Motorola MC68HC16 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC11    = 0x46, /* Motorola MC68HC11 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC08    = 0x47, /* Motorola MC68HC08 Microcontroller */
	ELF_ISA_MOTOROLA_MC68HC05    = 0x48, /* Motorola MC68HC05 Microcontroller */
	ELF_ISA_SGI_SVX              = 0x49, /* Silicon Graphics SVx */
	ELF_ISA_STM_ST19             = 0x4A, /* STMicroelectronics ST19 8-bit microcontroller */
	ELF_ISA_DIGITAL_VAX          = 0x4B, /* Digital VAX */
	ELF_ISA_AXIS_32_BIT          = 0x4C, /* Axis Communications 32-bit embedded processor */
	ELF_ISA_INFINEON_32_BIT      = 0x4D, /* Infineon Technologies 32-bit embedded processor */
	ELF_ISA_ELEMENT14_DSP        = 0x4E, /* Element 14 64-bit DSP Processor */
	ELF_ISA_LSI_LOGIC_DSP        = 0x4F, /* LSI Logic 16-bit DSP Processor */
	/* ELF_ISA 0x50 - 0x8B: Reserved for future use */
	ELF_ISA_TI_C6000             = 0x8C, /* TMS320C6000 Family */
	/* ELF_ISA 0x8D - 0xAE: Reserved for future use */
	ELF_ISA_MCST_ELBRUS_E2K      = 0xAF, /* MCST Elbrus e2k */
	/* ELF_ISA 0xB0 - 0xB6: Reserved for future use */
	ELF_ISA_ARM_64               = 0xB7, /* Arm 64-bits (Armv8/AArch64) */
	/* ELF_ISA 0xB8 - 0xDB: Reserved for future use */
	ELF_ISA_ZILOG_Z80            = 0xDC, /* Zilog Z80 */
	/* ELF_ISA 0xDD - 0xF2: Reserved for future use */
	ELF_ISA_RISC_V               = 0xF3, /* RISC-V */
	/* ELF_ISA 0xF4 - 0xF6: Reserved for future use */
	ELF_ISA_BPF                  = 0xF7, /* Berkeley Packet Filter */
	/* ELF_ISA 0xF8 - 0xFE: Reserved for future use */
	ELF_ISA_WDC_65C816           = 0x101 /* WDC 65C816 */
	/* ELF_ISA 0x102 - 0xFFFF: Reserved for future use */
};

enum elf_version {
	ELF_VER_1,
};

enum header_type {
	HEADER_FORMAT_ELF,
	HEADER_FORMAT_PE32,
	HEADER_FORMAT_UNKNOWN,
};
void elf32_swap_bytes(elf32_header *header);
void elf64_swap_bytes(elf32_header *header);

/*==================================================
 *                                                 *
 *                                                 *
 *                PE32 Format Begin                *
 *                                                 *
 *                                                 *
 *================================================*/

typedef struct {
	uint32_t magic;
	uint16_t machine;
	uint16_t number_of_sections;
	uint32_t time_date_stamp;
	uint32_t pointer_to_symbol_table; /* This should be 0 for image because COFF debug info is deprecated */
	uint32_t number_of_symbols;       /* This should be 0 for image because COFF debug info is deprecated */
	uint16_t size_of_optional_header;
	uint16_t characteristics;
} pe32_header;

#define PE32 0x010b
#define PE32PLUS 0x020b
typedef struct {
	uint16_t ident;			/* Is set to either PE32(32 bit) or PE32PLUS(64 bit) */
	uint8_t  major_linker_ver;		/* Major linker version */
	uint8_t  minor_linker_ver;		/* Minor linker version */
	uint32_t code_size;			/* Size of code(text) section or the sum of all code sections if there are multiple sections */
	uint32_t initialized_data_size;	/* The size of the initialized data section, or the sum of all such sections if there are multiple data sections */
	uint32_t uninitialized_data_size;	/* The size of the uninitialized data section (BSS), or the sum of all such sections if there are multiple BSS sections */
	uint32_t entry_point_address;		/* The address of the entry point relative to the image base when the executable file is loaded into memory. For program
						 * images, this is the starting address. For device drivers, this is the address of the initialization function. An entry
						 * point is optional for DLLs. When no entry point is present, this field must be zero. */
	uint32_t base_of_code;			/* The address that is relative to the image base of the beginning-of-code section when it is loaded into memory. */
	uint32_t base_of_data;			/* The address that is relative to the image base of the beginning-of-data section when it is loaded into memory. */
	uint32_t image_base;			/* The preferred address of the first byte of image when loaded into memory; must be a multiple of 64 K. The default for DLLs
						 * is 0x10000000. The default for Windows CE EXEs is 0x00010000. The default for Windows NT, Windows 2000, Windows XP,
						 * Windows 95, Windows 98, and Windows Me is 0x00400000. */
	uint32_t section_alignement;		/* The alignment (in bytes) of sections when they are loaded into memory. It must be greater than or equal to FileAlignment.
						 * The default is the page size for the architecture. */
	uint32_t file_alignement;		/* The alignment factor (in bytes) that is used to align the raw data of sections in the image file. The value should be a
						 * power of 2 between 512 and 64 K, inclusive. The default is 512. If the SectionAlignment is less than the architecture's
						 * page size, then FileAlignment must match SectionAlignment. */
	uint16_t major_operating_system_ver;	/* The major version number of the required operating system. */
	uint16_t minor_operating_system_ver;	/* The minor version number of the required operating system. */
	uint16_t major_image_ver;		/* The major version number of the image. */
	uint16_t minor_image_ver;		/* The minor version number of the image. */
	uint16_t major_subsystem_ver;		/* The major version number of the subsystem. */
	uint16_t minor_subsystem_ver;		/* The minor version number of the subsystem. */
	uint32_t win32_ver_value;		/* Reserved, must be zero. */
	uint32_t sizeof_image;			/* The size (in bytes) of the image, including all headers, as the image is loaded in memory. It must be a multiple of SectionAlignment. */
	uint32_t sizeof_headers;		/* The combined size of an MS-DOS stub, PE header, and section headers rounded up to a multiple of FileAlignment. */
	uint32_t checksum;			/* The combined size of an MS-DOS stub, PE header, and section headers rounded up to a multiple of FileAlignment. */
	uint16_t subsystem;			/* The subsystem that is required to run this image. */
	uint16_t dll_characteristics;
	uint32_t sizeof_stack_reserve;		/* The size of the stack to reserve. Only SizeOfStackCommit is committed; the rest is made available one page at a time until the reserve size is reached. */
	uint32_t sizeof_stack_commit;		/* The size of the stack to commit. */
	uint32_t sizeof_heap_reserve;		/* The size of the local heap space to reserve. Only SizeOfHeapCommit is committed; the rest is made available one page at a time until the reserve size is reached. */
	uint32_t sizeof_heap_commit;		/* The size of the local heap space to commit. */
	uint32_t laoder_flags;			/* Reserved, must be zero. */
	uint32_t numof_rva_and_sizes;		/* The number of data-directory entries in the remainder of the optional header. Each describes a location and size. */
} pe32_optional_header;

enum pe32_machine_type {
	PE32_MACHINE_TYPE_UNKNOWN    = 0x0,    /* The content of this field is assumed to be applicable to any machine type */
	PE32_MACHINE_TYPE_ALPHA      = 0x184,  /* Alpha AXP, 32-bit address space */
	PE32_MACHINE_TYPE_ALPHA64    = 0x284,  /* Alpha 64, 64-bit address space */
	PE32_MACHINE_TYPE_AM33       = 0x1d3,  /* Matsushita AM33 */
	PE32_MACHINE_TYPE_AMD64      = 0x8664, /* x64 */
	PE32_MACHINE_TYPE_ARM        = 0x1c0,  /* ARM little endian */
	PE32_MACHINE_TYPE_ARM64      = 0xaa64, /* ARM64 little endian */
	PE32_MACHINE_TYPE_ARMNT      = 0x1c4,  /* ARM Thumb-2 little endian */
	PE32_MACHINE_TYPE_AXP64      = 0x284,  /* AXP 64 (Same as Alpha 64) */
	PE32_MACHINE_TYPE_EBC        = 0xebc,  /* EFI byte code */
	PE32_MACHINE_TYPE_I386       = 0x14c,  /* Intel 386 or later processors and compatible processors */
	PE32_MACHINE_TYPE_IA64       = 0x200,  /* Intel Itanium processor family */
	PE32_MACHINE_TYPE_LOONGARCH32 = 0x6232,/* LoongArch 32-bit processor family */
	PE32_MACHINE_TYPE_LOONGARCH64 = 0x6264,/* LoongArch 64-bit processor family */
	PE32_MACHINE_TYPE_M32R       = 0x9041, /* Mitsubishi M32R little endian */
	PE32_MACHINE_TYPE_MIPS16     = 0x266,  /* MIPS16 */
	PE32_MACHINE_TYPE_MIPSFPU    = 0x366,  /* MIPS with FPU */
	PE32_MACHINE_TYPE_MIPSFPU16  = 0x466,  /* MIPS16 with FPU */
	PE32_MACHINE_TYPE_POWERPC    = 0x1f0,  /* Power PC little endian */
	PE32_MACHINE_TYPE_POWERPCFP  = 0x1f1,  /* Power PC with floating point support */
	PE32_MACHINE_TYPE_R4000      = 0x166,  /* MIPS little endian */
	PE32_MACHINE_TYPE_RISCV32    = 0x5032, /* RISC-V 32-bit address space */
	PE32_MACHINE_TYPE_RISCV64    = 0x5064, /* RISC-V 64-bit address space */
	PE32_MACHINE_TYPE_RISCV128   = 0x5128, /* RISC-V 128-bit address space */
	PE32_MACHINE_TYPE_SH3        = 0x1a2,  /* Hitachi SH3 */
	PE32_MACHINE_TYPE_SH3DSP     = 0x1a3,  /* Hitachi SH3 DSP */
	PE32_MACHINE_TYPE_SH4        = 0x1a6,  /* Hitachi SH4 */
	PE32_MACHINE_TYPE_SH5        = 0x1a8,  /* Hitachi SH5 */
	PE32_MACHINE_TYPE_THUMB      = 0x1c2,  /* Thumb */
	PE32_MACHINE_TYPE_WCEMIPSV2  = 0x169   /* MIPS little-endian WCE v2 */
};

static struct {
    enum pe32_machine_type machine_type;
    const char* str;
} pe32_machine_type_map[] = {
	{ PE32_MACHINE_TYPE_UNKNOWN, "Unknown machine type" },
	{ PE32_MACHINE_TYPE_ALPHA, "Alpha AXP, 32-bit address space" },
	{ PE32_MACHINE_TYPE_ALPHA64, "Alpha 64, 64-bit address space" },
	{ PE32_MACHINE_TYPE_AM33, "Matsushita AM33" },
	{ PE32_MACHINE_TYPE_AMD64, "x64" },
	{ PE32_MACHINE_TYPE_ARM, "ARM little endian" },
	{ PE32_MACHINE_TYPE_ARM64, "ARM64 little endian" },
	{ PE32_MACHINE_TYPE_ARMNT, "ARM Thumb-2 little endian" },
	{ PE32_MACHINE_TYPE_AXP64, "AXP 64 (Same as Alpha 64)" },
	{ PE32_MACHINE_TYPE_EBC, "EFI byte code" },
	{ PE32_MACHINE_TYPE_I386, "Intel 386 or later processors and compatible processor" },
	{ PE32_MACHINE_TYPE_IA64, "Intel Itanium processor family" },
	{ PE32_MACHINE_TYPE_LOONGARCH32, "LoongArch 32-bit processor family" },
	{ PE32_MACHINE_TYPE_LOONGARCH64, "LoongArch 64-bit processor family" },
	{ PE32_MACHINE_TYPE_M32R, "Mitsubishi M32R little endian" },
	{ PE32_MACHINE_TYPE_MIPS16, "MIPS16" },
	{ PE32_MACHINE_TYPE_MIPSFPU, "MIPS with FPU" },
	{ PE32_MACHINE_TYPE_MIPSFPU16, "MIPS16 with FPU" },
	{ PE32_MACHINE_TYPE_POWERPC, "Power PC little endian" },
	{ PE32_MACHINE_TYPE_POWERPCFP, "Power PC with floating point support" },
	{ PE32_MACHINE_TYPE_R4000, "MIPS little endian" },
	{ PE32_MACHINE_TYPE_RISCV32, "RISC-V 32-bit address space" },
	{ PE32_MACHINE_TYPE_RISCV64, "RISC-V 64-bit address space" },
	{ PE32_MACHINE_TYPE_RISCV128, "RISC-V 128-bit address space" },
	{ PE32_MACHINE_TYPE_SH3, "Hitachi SH3" },
	{ PE32_MACHINE_TYPE_SH3DSP, "Hitachi SH3 DSP" },
	{ PE32_MACHINE_TYPE_SH4, "Hitachi SH4" },
	{ PE32_MACHINE_TYPE_SH5, "Hitachi SH5" },
	{ PE32_MACHINE_TYPE_THUMB, "Thumb" },
	{ PE32_MACHINE_TYPE_WCEMIPSV2, "MIPS little-endian WCE v2" },
};

enum pe32_image_characteristics {
	PE32_CHARACTERISTIC_RELOCS_STRIPPED       = 0x0001, /* File does not contain base relocations. */
	PE32_CHARACTERISTIC_EXECUTABLE_IMAGE      = 0x0002, /* File is valid and can be run. */
	PE32_CHARACTERISTIC_LINE_NUMS_STRIPPED    = 0x0004, /* Deprecated: COFF line numbers have been removed. */
	PE32_CHARACTERISTIC_LOCAL_SYMS_STRIPPED   = 0x0008, /* Deprecated: COFF symbol table entries for local symbols have been removed. */
	PE32_CHARACTERISTIC_AGGRESSIVE_WS_TRIM    = 0x0010, /* Obsolete: Aggressively trim working set. */
	PE32_CHARACTERISTIC_LARGE_ADDRESS_AWARE   = 0x0020, /* Application can handle > 2-GB addresses. */
	PE32_CHARACTERISTIC_RESERVED_0040         = 0x0040, /* Reserved for future use. */
	PE32_CHARACTERISTIC_BYTES_REVERSED_LO     = 0x0080, /* Deprecated: Little endian - the least significant bit (LSB) precedes the most significant bit (MSB) in memory. */
	PE32_CHARACTERISTIC_32BIT_MACHINE         = 0x0100, /* Machine is based on a 32-bit-word architecture. */
	PE32_CHARACTERISTIC_DEBUG_STRIPPED        = 0x0200, /* Debugging information is removed from the image file. */
	PE32_CHARACTERISTIC_REMOVABLE_RUN_FROM_SWAP = 0x0400, /* If the image is on removable media, fully load it and copy it to the swap file. */
	PE32_CHARACTERISTIC_NET_RUN_FROM_SWAP     = 0x0800, /* If the image is on network media, fully load it and copy it to the swap file. */
	PE32_CHARACTERISTIC_SYSTEM               = 0x1000, /* The image file is a system file, not a user program. */
	PE32_CHARACTERISTIC_DLL                  = 0x2000, /* The image file is a dynamic-link library (DLL). Such files are considered executable files for almost all purposes, although they cannot be directly run. */
	PE32_CHARACTERISTIC_UP_SYSTEM_ONLY       = 0x4000, /* The file should be run only on a uniprocessor machine. */
	PE32_CHARACTERISTIC_BYTES_REVERSED_HI    = 0x8000  /* Deprecated: Big endian - the MSB precedes the LSB in memory. */
};

static struct {
	int index;
	char *str;
} pe32_characteristics_map[] = {
	{ PE32_CHARACTERISTIC_RELOCS_STRIPPED, "Stripped" },
	{ PE32_CHARACTERISTIC_EXECUTABLE_IMAGE, "Valid Executable" },
	{ PE32_CHARACTERISTIC_LINE_NUMS_STRIPPED, "COFF line nums are stripped" },
	{ PE32_CHARACTERISTIC_LOCAL_SYMS_STRIPPED, "COFF table entries are stripped" },
	{ PE32_CHARACTERISTIC_AGGRESSIVE_WS_TRIM, "Agressively trim working set" },
	{ PE32_CHARACTERISTIC_LARGE_ADDRESS_AWARE, "Can handle > 2GB adresses" },
	{ PE32_CHARACTERISTIC_RESERVED_0040, "" },
	{ PE32_CHARACTERISTIC_BYTES_REVERSED_LO, "Little endian" },
	{ PE32_CHARACTERISTIC_32BIT_MACHINE, "Based on 32 bit word architecture" },
	{ PE32_CHARACTERISTIC_DEBUG_STRIPPED, "Debug info is stripped" },
	{ PE32_CHARACTERISTIC_REMOVABLE_RUN_FROM_SWAP, "Copy image to swap if it is on removable media" },
	{ PE32_CHARACTERISTIC_NET_RUN_FROM_SWAP, "If the image is on network media, fully load it and copy it to the swap file." },
	{ PE32_CHARACTERISTIC_SYSTEM, "System file" },
	{ PE32_CHARACTERISTIC_DLL, "DLL file" },
	{ PE32_CHARACTERISTIC_UP_SYSTEM_ONLY, "Uniprocessor machine only" },
	{ PE32_CHARACTERISTIC_BYTES_REVERSED_HI, "Big endian" },
};

enum pe32_subsystem {
	PE32_SUBSYSTEM_UNKNOWN                 = 0,  /* An unknown subsystem */
	PE32_SUBSYSTEM_NATIVE                  = 1,  /* Device drivers and native Windows processes */
	PE32_SUBSYSTEM_WINDOWS_GUI             = 2,  /* The Windows graphical user interface (GUI) subsystem */
	PE32_SUBSYSTEM_WINDOWS_CUI             = 3,  /* The Windows character subsystem */
	PE32_SUBSYSTEM_OS2_CUI                 = 5,  /* The OS/2 character subsystem */
	PE32_SUBSYSTEM_POSIX_CUI               = 7,  /* The Posix character subsystem */
	PE32_SUBSYSTEM_NATIVE_WINDOWS          = 8,  /* Native Win9x driver */
	PE32_SUBSYSTEM_WINDOWS_CE_GUI          = 9,  /* Windows CE */
	PE32_SUBSYSTEM_EFI_APPLICATION         = 10, /* An Extensible Firmware Interface (EFI) application */
	PE32_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11, /* An EFI driver with boot services */
	PE32_SUBSYSTEM_EFI_RUNTIME_DRIVER      = 12, /* An EFI driver with run-time services */
	PE32_SUBSYSTEM_EFI_ROM                 = 13, /* An EFI ROM image */
	PE32_SUBSYSTEM_XBOX                    = 14, /* XBOX */
	PE32_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16  /* Windows boot application */
};

static struct {
	int index;
	char *str;
} pe32_subsystem_map[] = {
	{ PE32_SUBSYSTEM_UNKNOWN, "Unknown subsystem" },
	{ PE32_SUBSYSTEM_NATIVE, "Native"},
	{ PE32_SUBSYSTEM_WINDOWS_GUI, "GUI"},
	{ PE32_SUBSYSTEM_WINDOWS_CUI, "CUI character" },
	{ PE32_SUBSYSTEM_OS2_CUI, "OS/2 character"},
	{ PE32_SUBSYSTEM_POSIX_CUI, "POSIX character" },
	{ PE32_SUBSYSTEM_NATIVE_WINDOWS, "Native Win9x driver" },
	{ PE32_SUBSYSTEM_WINDOWS_CE_GUI, "Windows CE" },
	{ PE32_SUBSYSTEM_EFI_APPLICATION, "EFI" },
	{ PE32_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER, "EFI boot service"},
	{ PE32_SUBSYSTEM_EFI_RUNTIME_DRIVER, "EFI runtime service"},
	{ PE32_SUBSYSTEM_EFI_ROM, "EFI ROM image" },
	{ PE32_SUBSYSTEM_XBOX, "XBOX"},
	{ PE32_SUBSYSTEM_WINDOWS_BOOT_APPLICATION, "Windows boot"},
};

enum pe32_dll_characteristics {
	PE32_DLLCHARACTERISTICS_HIGH_ENTROPY_VA     = 0x0020, /* Image can handle a high entropy 64-bit virtual address space. */
	PE32_DLLCHARACTERISTICS_DYNAMIC_BASE        = 0x0040, /* DLL can be relocated at load time. */
	PE32_DLLCHARACTERISTICS_FORCE_INTEGRITY     = 0x0080, /* Code Integrity checks are enforced. */
	PE32_DLLCHARACTERISTICS_NX_COMPAT           = 0x0100, /* Image is NX compatible. */
	PE32_DLLCHARACTERISTICS_NO_ISOLATION        = 0x0200, /* Isolation aware, but do not isolate the image. */
	PE32_DLLCHARACTERISTICS_NO_SEH              = 0x0400, /* Does not use structured exception (SE) handling. No SE handler may be called in this image. */
	PE32_DLLCHARACTERISTICS_NO_BIND             = 0x0800, /* Do not bind the image. */
	PE32_DLLCHARACTERISTICS_APPCONTAINER        = 0x1000, /* Image must execute in an AppContainer. */
	PE32_DLLCHARACTERISTICS_WDM_DRIVER          = 0x2000, /* A WDM driver. */
	PE32_DLLCHARACTERISTICS_GUARD_CF            = 0x4000, /* Image supports Control Flow Guard. */
	PE32_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE = 0x8000 /* Terminal Server aware. */
};

static struct {
	int index;
	char *str;
} pe32_dll_characteristics_map[] = {
	{ PE32_DLLCHARACTERISTICS_HIGH_ENTROPY_VA,      "Image can handle a high entropy 64-bit virtual address space." },
	{ PE32_DLLCHARACTERISTICS_DYNAMIC_BASE,         "DLL can be relocated at load time." },
	{ PE32_DLLCHARACTERISTICS_FORCE_INTEGRITY,      "Code Integrity checks are enforced." },
	{ PE32_DLLCHARACTERISTICS_NX_COMPAT,            "Image is NX compatible." },
	{ PE32_DLLCHARACTERISTICS_NO_ISOLATION,         "Isolation aware, but do not isolate the image." },
	{ PE32_DLLCHARACTERISTICS_NO_SEH,               "Does not use structured exception (SE) handling. No SE handler may be called in this image." },
	{ PE32_DLLCHARACTERISTICS_NO_BIND,              "Do not bind the image." },
	{ PE32_DLLCHARACTERISTICS_APPCONTAINER,         "Image must execute in an AppContainer." },
	{ PE32_DLLCHARACTERISTICS_WDM_DRIVER,           "A WDM driver." },
	{ PE32_DLLCHARACTERISTICS_GUARD_CF,             "Image supports Control Flow Guard." },
	{ PE32_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE, "Terminal Server aware." }
};

#define FILE_FORMAT_PE32_SIGNATURE_EXISTS 0x00004550

int pe32_does_signature_exist(char *data);

const char* pe32_get_machine_type_string(enum pe32_machine_type machine_type);
const char* pe32_get_characteristics_string(enum pe32_image_characteristics machine_type);
const char* pe32_get_subsystem_string(enum pe32_subsystem type);
const char* pe32_get_dll_characteristics_string(enum pe32_subsystem type);

int pe32_is_flag_set(uint16_t characteristics, enum pe32_image_characteristics bit);
pe32_header *pe32_get_header(void *data);
pe32_optional_header *pe32_get_optional_header(void *data);

int determine_link_format(char *data);

#endif /* FILE_FORMAT_H */
