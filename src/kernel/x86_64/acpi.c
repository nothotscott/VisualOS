/*
 * File:		acpi.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "acpi.h"

struct SDTHeader* acpi_get_table(struct SDTHeader* xsdt, char* signature) {
	for(size_t t = 0; t < xsdt->length - sizeof(struct SDTHeader); t += 8){
		struct SDTHeader* header = (struct SDTHeader*)*(uint64_t*)((uint64_t)xsdt + sizeof(struct SDTHeader) + t);
		for(int i = 0; i < 4; i++) {
			if(header->signature[i] != signature[i]) {
				break;
			}
			if(i == 3) {
				return header;
			}
		}
	}
	return NULL;
}