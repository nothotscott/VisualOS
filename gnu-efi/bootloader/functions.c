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

struct PSF1Font* load_psf1_font(EFI_FILE* directory, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
	EFI_FILE* font_file = load_file(directory, path, ImageHandle, SystemTable);
	struct PSF1Header* font_header;
	UINTN header_size = sizeof(struct PSF1Header);
	if(font_file == NULL){
		return NULL;
	}
	// Load font header
	SystemTable->BootServices->AllocatePool(EfiLoaderData, header_size, (void**)&font_header);
	font_file->Read(font_file, &header_size, font_header);
	if(font_header->magic[0] != PSF1_MAGIC0 || font_header->magic[1] != PSF1_MAGIC1){
		return NULL;
	}
	// Load glyph
	UINTN glyph_buffer_size = font_header->charsize * 256;
	if(font_header->mode == 1){	// larger glyph mode
		glyph_buffer_size = font_header->charsize * 512;
	}
	void* glyph_buffer;
	{
		font_file->SetPosition(font_file, header_size);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyph_buffer_size, (void**)&glyph_buffer);
		font_file->Read(font_file, &glyph_buffer_size, glyph_buffer);
	}
	// Finish up
	struct PSF1Font* font;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(struct PSF1Font), (void**)&font);
	font->header_ptr = font_header;
	font->glyph_buffer = glyph_buffer;
	return font;
}

struct TGAImage* load_tga_image(EFI_FILE* directory, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
	EFI_FILE* file = load_file(directory, path, ImageHandle, SystemTable);
	struct TGAHeader* header;
	UINTN header_size = sizeof(struct TGAHeader);
	if(file == NULL){
		return NULL;
	}
	// Load image header
	SystemTable->BootServices->AllocatePool(EfiLoaderData, header_size, (void**)&header);
	file->Read(file, &header_size, header);
	// Load image
	UINTN buffer_size = header->width * header->height * header->bbp / 8;
	void* buffer;
	{
		file->SetPosition(file, header_size);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, buffer_size, (void**)&buffer);
		file->Read(file, &buffer_size, buffer);
	}
	// Finish up
	struct TGAImage* image;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(struct TGAImage), (void**)&image);
	image->header_ptr = header;
	image->buffer = buffer;
	return image;
}