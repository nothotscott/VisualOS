include Make.defaults

SUBDIRS			:= kernel
EFI_BIN			:= $(BUILD_DIR)/gnu-efi/bootloader/BOOTX64.efi
KERNEL_BIN		:= $(OBJ_DIR)/kernel.elf
STARTUP_SCRIPT	:= gnu-efi/startup.nsh

all:			$(SUBDIRS) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
				@mkdir -p $(OBJ_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):		setup
				@echo "Building $@"
				$(MAKE) --no-print-directory -f $@/Makefile THIS=$@

.PHONY: gnu-efi
gnu-efi:		setup
				@echo "Building $@"
				cd $@ && $(MAKE) THIS=$@ BUILD_DIR=$(BUILD_DIR) OBJ_DIR=$(OBJ_DIR)

$(BUILD_DIR)/$(OSNAME).img:	$(KERNEL_BIN)
				dd if=/dev/zero of=$@ bs=512 count=93750
				mformat -i $@ -f 1440 ::
				mmd -i $@ ::/EFI
				mmd -i $@ ::/EFI/BOOT
#				mmd -i $@ ::/extras
				mcopy -i $@ $(KERNEL_BIN) ::
				mcopy -i $@ $(EFI_BIN) ::/EFI/BOOT
				mcopy -i $@ $(STARTUP_SCRIPT) ::
				mcopy -i $@ -s extras/ ::/extras
.PHONY: img
img:			$(BUILD_DIR)/$(OSNAME).img

$(BUILD_DIR)/$(OSNAME).iso:	img
				mkdir -p $(BUILD_DIR)/iso
				cp $(BUILD_DIR)/$(OSNAME).img $(BUILD_DIR)/iso
				xorriso -as mkisofs -R -f -e $(OSNAME).img -no-emul-boot -o $(BUILD_DIR)/$(OSNAME).iso $(BUILD_DIR_ABS)/iso
				rm -rf $(BUILD_DIR)/iso

.PHONY: iso
iso:			$(BUILD_DIR)/$(OSNAME).iso


.PHONY: clean-obj
clean-obj:		
				cd $(OBJ_DIR) && rm -rf *.o *.so *.elf

.PHONY: clean-img
clean-img:		
				cd $(BUILD_DIR) && rm -rf *.img *.iso

.PHONY: clean-gnuefi
clean-gnuefi:	clean-img
				rm -rf $(BUILD_DIR)/gnu-efi/bootloader/*.*

.PHONY: clean-kernel
clean-kernel:	clean-obj

.PHONY: clean-dev
clean-dev:		clean-gnuefi clean-kernel clean-img

.PHONY: clean
clean:		
				find $(BUILD_DIR) -type f -name '*.*' -delete

.PHONY: clean-super
clean-super:	
				rm -rf $(BUILD_DIR)