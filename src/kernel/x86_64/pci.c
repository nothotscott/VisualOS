/*
 * File:		pci.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "memory/paging.h"
#include "acpi.h"
#include "pci.h"


static struct PCIDeviceHeader* s_pci_devices[PCI_MAX_LANES];
static size_t s_pci_devices_num = 0;


void pci_init(struct MCFGHeader* mcfg) {
	//printf("MCFG Length %d\n", mcfg->length);
	for(size_t t = 0; t < mcfg->length - sizeof(struct MCFGHeader); t += sizeof(struct PCIDeviceConfigurationDescriptor)){
		struct PCIDeviceConfigurationDescriptor* descriptor = (struct PCIDeviceConfigurationDescriptor*)((uint64_t)mcfg + sizeof(struct MCFGHeader) + t);
		for(uint8_t bus_i = descriptor->bus_start; bus_i < descriptor->bus_end; bus_i++) {
			void* bus = (void*)descriptor->base + ((uint64_t)bus_i << 20);
			paging_identity_map_page(bus);
			struct PCIDeviceHeader* pci_header = (struct PCIDeviceHeader*)bus;
			if(pci_header->device_id == 0 || pci_header->device_id == 0xffff) {
				// Invalid device
				continue;
			}
			for(uint8_t device_i = 0; device_i < PCI_MAX_DEVICES; device_i++) {
				void* device = bus + ((uint64_t)device_i << 15);
				paging_identity_map_page(device);
				struct PCIDeviceHeader* pci_header = (struct PCIDeviceHeader*)device;
				if(pci_header->device_id == 0 || pci_header->device_id == 0xffff) {
					continue;
				}
				for(uint8_t function_i = 0; function_i < PCI_MAX_FUNCTIONS; function_i++) {
					void* function = device + ((uint64_t)function_i << 12);
					paging_identity_map_page(function);
					struct PCIDeviceHeader* pci_header = (struct PCIDeviceHeader*)function;
					if(pci_header->device_id == 0 || pci_header->device_id == 0xffff) {
						continue;
					}
					s_pci_devices[s_pci_devices_num] = pci_header;
					s_pci_devices_num++;
					//printf("Detected PCI Device: 0x%x\n", pci_header->device_id);
				}
			}
		}
	}
}

void pci_print() {
	for(size_t i = 0; i < s_pci_devices_num; i++){
		struct PCIDeviceHeader* pci_header = s_pci_devices[i];
		printf("PCI Device: 0x%x, Vendor Name: %s\n", pci_header->device_id, pci_get_vendor_str(pci_header));
		printf("  [%s]->[%s]->[%s]\n", pci_get_class_str(pci_header), pci_get_subclass_str(pci_header), pci_get_progif_str(pci_header));
	}
}