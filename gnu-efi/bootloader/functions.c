/*
 * File:		functions.c
 * Description:	EFI specific functions
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */
#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

typedef unsigned long long	size_t;

int mem_compare(const void* aptr, const void* bptr, size_t n) {
	const unsigned char* a = aptr;
	const unsigned char* b = bptr;
	for (size_t i = 0l; i < n; i++){
		if (a[i] < b[i]){
			return -1;
		} else if (a[i] > b[i]){
			return 1;
		}
	}
	return 0;
}

EFI_FILE* load_file(EFI_FILE* directory, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* loaded_file;
	EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* file_system;
	// Load
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
	SystemTable->BootServices->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_system);
	// Check if directory is null, and open the root of the file system if so
	if(directory == NULL){
		file_system->OpenVolume(file_system, &directory);
	}
	// Open directory and save into loaded_file
	EFI_STATUS s = directory->Open(directory, &loaded_file, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if(s != EFI_SUCCESS){
		return NULL;
	}
	return loaded_file;
}