#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long	size_t;

EFI_FILE* load_file(EFI_FILE* dir, CHAR16* path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* loaded_file;
	EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* file_system;
	// Load
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
	SystemTable->BootServices->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&file_system);
	// Check if directory is null, and open the root of the file system if so
	if(dir == NULL){
		file_system->OpenVolume(file_system, &dir);
	}
	// Open directory and save into loaded_file
	EFI_STATUS s = dir->Open(dir, &loaded_file, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if(s != EFI_SUCCESS){
		return NULL;
	}
	return loaded_file;
}

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

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
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
	if( mem_compare(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
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
				Print(L"Kernel program header read (Unknown)\n\r");
			}
		}
	}
	int (*kernel_start)() = ((__attribute__((sysv_abi)) int(*)()) header.e_entry);
	Print(L"%d\n\r", kernel_start());

	return EFI_SUCCESS; // Exit the UEFI application
}
