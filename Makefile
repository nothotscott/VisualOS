include Make.defaults

EFI_BIN			:= $(BUILD_DIR)/gnu-efi/bootloader/BOOTX64.efi
KERNEL_BIN		:= $(KERNEL_DIR)/kernel.elf
STARTUP_SCRIPT	:= $(SRC_DIR)/$(BOOTLOADER)/startup.nsh

all:			$(SUBDIRS) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
				@mkdir -p $(KERNEL_DIR)
				@mkdir -p $(LIB_DIR)
ifdef ASMDUMP
				@mkdir -p $(ASMDUMP_DIR)
endif

###############################################################################

MAKE_VOS	= $(MAKE) --no-print-directory -f $(SRC_DIR)/$@/Makefile THIS=$(SRC_DIR)/$@

.PHONY: $(BOOTLOADER)
$(BOOTLOADER):	setup
				@echo "Building $@"
				cd $(SRC_DIR)/$@ && $(MAKE) THIS=$@ BUILD_DIR=$(BUILD_DIR_ABS) KERNEL_SRC=$(SRC_DIR)/$(KERNEL)

.PHONY: $(LIBRARY)
$(LIBRARY):		setup
				@echo "Building $@"
				$(MAKE_VOS) TARGET_DIR=$(LIB_DIR)

.PHONY: $(KERNEL)
$(KERNEL):		setup $(LIBRARY)
				@echo "Building $@"
				$(MAKE_VOS) TARGET_DIR=$(KERNEL_DIR)

###############################################################################


$(BUILD_DIR)/$(OSNAME).img:	$(KERNEL_BIN)
				dd if=/dev/zero of=$@ bs=512 count=93750
				mformat -i $@ -f 1440 ::
				mmd -i $@ ::/EFI
				mmd -i $@ ::/EFI/BOOT
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

###############################################################################

RM_OBJ	:= rm -rf *.o *.a *.so *.elf
RM_IMG	:= rm -rf *.img *.iso *.vdi

.PHONY: clean-gnuefi
clean-gnuefi:	clean-img
				rm -rf $(BUILD_DIR)/gnu-efi/bootloader

.PHONY: clean-vos
clean-vos:		clean-img
				rm -rf $(LIB_DIR)/
				rm -rf $(KERNEL_DIR)/
ifneq "$(wildcard $(ASMDUMP_DIR))" ""
				cd $(ASMDUMP_DIR) && rm -rf *.s
endif

.PHONY: clean-img
clean-img:		
				cd $(BUILD_DIR) && $(RM_IMG)

.PHONY: clean
clean:		
				rm -rf $(BUILD_DIR)