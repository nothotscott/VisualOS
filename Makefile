include Make.defaults

SUBDIRS			:= kernel
EFI_BIN			:= $(BUILD_DIR)/gnu-efi/bootloader/main.efi
KERNEL_BIN		:= $(OBJ_DIR)/kernel.elf
STARTUP_SCRIPT	:= gnu-efi/startup.nsh

all:			$(SUBDIRS) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
				@mkdir -p $(OBJ_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
				@echo "Building $@"
				$(MAKE) -f $@/Makefile THIS=$@

.PHONY: gnu-efi
gnu-efi:
				@echo "Building $@"
				cd $@ && $(MAKE) THIS=$@ BUILD_DIR=$(BUILD_DIR) OBJ_DIR=$(OBJ_DIR)

$(BUILD_DIR)/$(OSNAME).img:
				dd if=/dev/zero of=$@ bs=512 count=93750
				mformat -i $@ -f 1440 ::
				mmd -i $@ ::/EFI
				mmd -i $@ ::/EFI/BOOT
				mcopy -i $@ $(EFI_BIN) ::/EFI/BOOT
				mcopy -i $@ $(STARTUP_SCRIPT) ::
				mcopy -i $@ $(KERNEL_BIN) ::
.PHONY: img
img:			$(BUILD_DIR)/$(OSNAME).img

$(BUILD_DIR)/$(OSNAME).iso:	img
				mkdir -p $(BUILD_DIR)/iso
				cp $(BUILD_DIR)/$(OSNAME).img $(BUILD_DIR)/iso
				xorriso -as mkisofs -R -f -e $(OSNAME).img -no-emul-boot -o $(BUILD_DIR)/$(OSNAME).iso $(BUILD_DIR_ABS)/iso
				rm -rf $(BUILD_DIR)/iso

.PHONY: iso
iso:			$(BUILD_DIR)/$(OSNAME).iso


.PHONY: objclean
objclean:		
				cd $(OBJ_DIR) && rm -rf *.o *.so *.elf

.PHONY: imgclean
imgclean:		
				cd $(BUILD_DIR) && rm -rf *.img *.iso

.PHONY: gnueficlean
gnueficlean:	imgclean
				rm -rf $(BUILD_DIR)/gnu-efi/bootloader/*.*

.PHONY: kernelclean
kernelclean:	objclean

.PHONY: devclean
devclean:		gnueficlean kernelclean

.PHONY: clean
clean:		
				find $(BUILD_DIR) -type f -name '*.*' -delete

.PHONY: superclean
superclean:	
				rm -rf $(BUILD_DIR)