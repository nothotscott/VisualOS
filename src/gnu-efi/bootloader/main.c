/*
 * File:		main.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long	size_t;

#define DEBUG_MODE		0
#define SYSTEM_FONT		L"zap-ext-light18.psf"
#define SYSTEM_BMP		L"VisualOS.bmp"
#define SYSTEM_TGA		L"VisualOS.tga"

// Headers for functions.c
int mem_compare(const void*, const void*, unsigned long long);
EFI_FILE* load_file(EFI_FILE*, CHAR16*, EFI_HANDLE, EFI_SYSTEM_TABLE*);
struct PSF1Font* load_psf1_font(EFI_FILE*, CHAR16*, EFI_HANDLE, EFI_SYSTEM_TABLE*);
struct TGAImage* load_tga_image(EFI_FILE*, CHAR16*, EFI_HANDLE, EFI_SYSTEM_TABLE*);

// Global structs
struct FrameBuffer g_frame_buffer;
struct KernelEntryInterface g_interface;

// Main code
void initalize_gop(){
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	UINTN info_size, num_modes, native_mode, current_mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol, 3, &gop_guid, NULL, (void**)&gop);
	if(EFI_ERROR(status)){
		Print(L"Cannot locate GOP\n\r");
		return;
	}
	Print(L"GOP located\n\r");
	status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode == NULL ? 0 : gop->Mode->Mode, &info_size, &info);
	if(status == EFI_NOT_STARTED) {
		status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
	}
	if(EFI_ERROR(status)){
		Print(L"Could not get current GOP mode\n\r");
		return;
	}
	native_mode = gop->Mode->Mode;
	current_mode = native_mode;
    num_modes = gop->Mode->MaxMode;
	for(UINTN i = 0; i < num_modes; i++){
		// Switch mode if needed here
		status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &info_size, &info);
		if(DEBUG_MODE){
			Print(L"  GOP mode %03d: Size=%dx%d, Format=%x%s\n\r",
				i, info->HorizontalResolution, info->VerticalResolution, info->PixelFormat, i == native_mode ? L" (native)" : L"");
		}
	}
	// Set frame buffer for the mode
	if(current_mode != native_mode) {
		status = uefi_call_wrapper(gop->SetMode, 2, gop, native_mode);
		if(EFI_ERROR(status)){
			Print(L"Could not set GOP mode\n\r");
			return;
		}
	}
	g_frame_buffer.base = (void*)gop->Mode->FrameBufferBase;
	g_frame_buffer.size  = gop->Mode->FrameBufferSize;
	g_frame_buffer.width  = gop->Mode->Info->HorizontalResolution;
	g_frame_buffer.height  = gop->Mode->Info->VerticalResolution;
	g_frame_buffer.ppsl  = gop->Mode->Info->PixelsPerScanLine;
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	// Begin loading the kernel
	InitializeLib(ImageHandle, SystemTable);	// UEFI environment to make out lives easier
	Print(L"Loading VisualOS\n\r");

	EFI_FILE* kernel = load_file(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (kernel == NULL){
		Print(L"Could not load kernel\n\r");
		return EFI_LOAD_ERROR;
	}
	Print(L"Loaded kernel\n\r");

	// Load binary header and check
	Elf64_Ehdr header;
	{
		UINTN file_info_size;
		EFI_FILE_INFO* file_info;
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &file_info_size, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, file_info_size, (void**)&file_info);
		kernel->GetInfo(kernel, &gEfiFileInfoGuid, &file_info_size, (void**)&file_info);
		UINTN size = sizeof(header);
		kernel->Read(kernel, &size, &header);
	}
	if(	mem_compare(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	){
		Print(L"Kernel format is bad\n\r");
		return EFI_UNSUPPORTED;
	}
	Print(L"Kernel verified\n\r");

	// Load binary program headers
	Elf64_Phdr* pheaders;
	{
		kernel->SetPosition(kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&pheaders);
		kernel->Read(kernel, &size, pheaders);
	}
	for( Elf64_Phdr* ph_ptr = pheaders;
		(char*)ph_ptr < (char*)pheaders + header.e_phnum * header.e_phentsize;
		ph_ptr = (Elf64_Phdr*)((char*)ph_ptr + header.e_phentsize)
	){
		switch(ph_ptr->p_type) {
			case PT_LOAD: {
				int pages = (ph_ptr->p_memsz + 0x1000 - 1) / 0x1000;	// get size to nearest 4K and round up
				Elf64_Addr seg = ph_ptr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &seg);
				kernel->SetPosition(kernel, ph_ptr->p_offset);
				UINTN size = ph_ptr->p_filesz;
				kernel->Read(kernel, &size, (void*)seg);
				Print(L"Kernel program header read (PT_LOAD)\n\r");
				break;
			}
			default: {
				Print(L"Kernel program header read (Unknown=%u)\n\r", ph_ptr->p_type);
			}
		}
	}
	// Initalize GOP
	initalize_gop();
	Print(L"GOP frame buffer data:\n\r"
		"  Base=0x%x\n\r"
		"  Size=%x\n\r"
		"  Width=%d\n\r"
		"  Height=%d\n\r"
		"  Pixels per scanline=%d\n\r", 
	g_frame_buffer.base, g_frame_buffer.size, g_frame_buffer.width, g_frame_buffer.height, g_frame_buffer.ppsl);

	EFI_FILE* extras = load_file(NULL, L"extras", ImageHandle, SystemTable);
	// Get font
	struct PSF1Font* font = load_psf1_font(extras, SYSTEM_FONT, ImageHandle, SystemTable);
	if(font == NULL){
		Print(L"Could not load font\n\r");
	}else{
		Print(L"Font found: Charsize=%d\n\r", font->header_ptr->charsize);
	}
	// Get TGA image
	struct TGAImage* image = load_tga_image(extras, SYSTEM_TGA, ImageHandle, SystemTable);
	if(image == NULL){
		Print(L"Could not find system TGA image\n\r");
	}else{
		Print(L"System TGA image found: Dimensions=%dx%d, Image type=%d, Bits per pixel=%d\n\r",
			image->header_ptr->width, image->header_ptr->height, image->header_ptr->image_type, image->header_ptr->bbp
		);
	}
	// Get memory descriptor
	EFI_MEMORY_DESCRIPTOR* map;
	UINTN map_size, map_key, descriptor_size;
	UINT32 descriptor_version;
	{
		SystemTable->BootServices->GetMemoryMap(&map_size, map, &map_key, &descriptor_size, &descriptor_version);
		Print(L"Memory map at 0x%x\n\r", map);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, map_size, (void**)&map);
		Print(L"Memory map size %d\n\r", map_size);
		SystemTable->BootServices->GetMemoryMap(&map_size, map, &map_key, &descriptor_size, &descriptor_version);
		Print(L"Memory map descriptor size %d\n\r", map_size);
	}
	//SystemTable->RuntimeServices->SetVirtualAddressMap(map_size, descriptor_size, descriptor_version, map);

	// Prepare for kernel space
	if(!DEBUG_MODE){
		uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
	}

	// Get important values for the kernel
	g_interface = (struct KernelEntryInterface){
		.frame_buffer = &g_frame_buffer,
		.font = font,
		.img = image,
		.mem_map = (struct MemoryDescriptor*)map,
		.mem_map_size = map_size,
		.mem_map_descriptor_size = descriptor_size
	};

	// Exit the UEFI application
	SystemTable->BootServices->ExitBootServices(ImageHandle, map_key);
	// Enter into the kernel
	// ?? Test for drawing to the GOP after Boot services have exited
	/*UINT32 pixel = 0xff80a0f0;
	UINT32* pixel_ptr = (UINT32*)g_frame_buffer.base;
	for(UINT32 x = 0; x < 64; x++){
		for(UINT32 y = 0; y < 64; y++){
			*(pixel_ptr + x + (y * g_frame_buffer.ppsl)) = pixel;
		}
	}*/
	void (*kernel_start)(struct KernelEntryInterface*) = ((__attribute__((sysv_abi)) void(*)()) header.e_entry);
	if(!DEBUG_MODE){
		kernel_start(&g_interface);
	}

	return EFI_SUCCESS;
}
