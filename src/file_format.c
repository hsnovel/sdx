// This file is a part of sdx libraries
// https://github.com/xcatalyst/sdx
//
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

#include "file_format.h"

int __swap_uint16(uint16_t byte)
{
	return ((byte>>8) | (byte<<8));
}

int __swap_uint32(uint32_t byte)
{
	return ((byte>>24)&0xff) | ((byte<<8)&0xff0000) |
		((byte>>8)&0xff00) | ((byte<<24)&0xff000000);
}

uint64_t __swap_uint64(uint64_t byte) {
	return ((byte >> 56) & 0x00000000000000FF) |
		((byte >> 40) & 0x000000000000FF00) |
		((byte >> 24) & 0x0000000000FF0000) |
		((byte >> 8)  & 0x00000000FF000000) |
		((byte << 8)  & 0x000000FF00000000) |
		((byte << 24) & 0x0000FF0000000000) |
		((byte << 40) & 0x00FF000000000000) |
		((byte << 56) & 0xFF00000000000000);
}

void elf32_swap_bytes(elf32_header *header)
{
	header->type = __swap_uint16(header->type);
	header->isa = __swap_uint16(header->isa);
	header->version = __swap_uint32(header->version);
	header->entry = __swap_uint32(header->entry);
	header->phoff = __swap_uint32(header->phoff);
	header->shoff = __swap_uint32(header->shoff);
	header->flags = __swap_uint32(header->flags);
	header->ehsize = __swap_uint16(header->ehsize);
	header->phentsize = __swap_uint16(header->phentsize);
	header->phnum = __swap_uint16(header->phnum);
	header->shentsize = __swap_uint16(header->shentsize);
	header->shnum = __swap_uint16(header->shnum);
	header->shstrndx = __swap_uint16(header->shstrndx);
}

void elf64_swap_bytes(elf32_header *header)
{
	header->type = __swap_uint16(header->type);
	header->isa = __swap_uint16(header->isa);
	header->version = __swap_uint32(header->version);
	header->entry = __swap_uint64(header->entry);
	header->phoff = __swap_uint64(header->phoff);
	header->shoff = __swap_uint64(header->shoff);
	header->flags = __swap_uint32(header->flags);
	header->ehsize = __swap_uint16(header->ehsize);
	header->phentsize = __swap_uint16(header->phentsize);
	header->phnum = __swap_uint16(header->phnum);
	header->shentsize = __swap_uint16(header->shentsize);
	header->shnum = __swap_uint16(header->shnum);
	header->shstrndx = __swap_uint16(header->shstrndx);
}

/**
 * Check if signature bytes PE\0\0 exist at index 0x3c
 * Signature only exists on image files and not object files
 * so this may be used to check the file type as well.
 *
 * Note that this value can also be checked with pe32_header
 * struct, the 'magic' value at the struct should be compared
 * agains FILE_FORMAT_PE32_SIGNATURE_EXISTS
 *
 * @return {int}: Returns 1 if exists, otherwise 0 is returned.
 */
int pe32_does_signature_exist(char *data)
{
	return (*(uint32_t*)(((uint8_t*)data) + 0xE8) == FILE_FORMAT_PE32_SIGNATURE_EXISTS);
}

/**
 * Get machine type string, this info is
 * extracted from pe32_header.machine
 *
 * return {char*}: Return the corresponding string for machine type
 */
const char* pe32_get_machine_type_string(enum pe32_machine_type machine_type) {
	for (size_t i = 0; i < sizeof(pe32_machine_type_map) / sizeof(pe32_machine_type_map[0]); ++i) {
		if (pe32_machine_type_map[i].machine_type == machine_type) {
			return pe32_machine_type_map[i].str;
		}
	}
	return "Invalid machine type";
}

const char* pe32_get_characteristics_string(enum pe32_image_characteristics machine_type)
{
	for (size_t i = 0; i < sizeof(pe32_characteristics_map) / sizeof(pe32_characteristics_map[0]); ++i) {
		if (pe32_characteristics_map[i].index == machine_type) {
			return pe32_characteristics_map[i].str;
		}
	}
	return "Invalid characteristics";
}

const char* pe32_get_subsystem_string(enum pe32_subsystem type)
{
	for (size_t i = 0; i < sizeof(pe32_subsystem_map) / sizeof(pe32_subsystem_map[0]); ++i) {
		if (pe32_subsystem_map[i].index == type) {
			return pe32_subsystem_map[i].str;
		}
	}
	return "Invalid subsystem";
}

const char* pe32_get_dll_characteristics_string(enum pe32_subsystem type)
{
	for (size_t i = 0; i < sizeof(pe32_dll_characteristics_map) / sizeof(pe32_dll_characteristics_map[0]); ++i) {
		if (pe32_dll_characteristics_map[i].index == type) {
			return pe32_dll_characteristics_map[i].str;
		}
	}
	return "No dll characteristics found";
}

/**
 * Check if flag is set in pe32_header.characteristics
 *
 * return {int}: Return 1 if flag is set, otherwise return 0.
 */
inline int pe32_is_flag_set(uint16_t characteristics, enum pe32_image_characteristics bit)
{
	return !!(characteristics & bit);
}

pe32_header *pe32_get_header(void *data)
{
	return (pe32_header*)(((uint8_t*)data) + 0xE8);
}

pe32_optional_header *pe32_get_optional_header(void *data)
{
	return (pe32_optional_header*)((uint8_t*)pe32_get_header(data) + sizeof(pe32_header));
}

/* Determine if file link format */
int determine_link_format(char *data)
{
	if(data[0] == 0x7f && data[1] == 'E' &&
	   data[2] == 'L' && data[3] == 'F') {
		return HEADER_FORMAT_ELF;
	}

	if ((data[0] == 'M') && (data[1] == 'Z'))
		return HEADER_FORMAT_PE32;

	return HEADER_FORMAT_UNKNOWN;
}
