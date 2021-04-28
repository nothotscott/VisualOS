THIS		:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include		Make.defaults

TARGET_DIR	= $(BUILD_DIR)

EFI_BIN			:= $(BUILD_DIR)/gnu-efi/bootloader/BOOTX64.efi
KERNEL_BIN		:= $(KERNEL_DIR)/kernel.elf
VOS_BIN			:= $(BUILD_DIR)/vos.elf

LDFLAGS	:= -T $(SRC_DIR)/vos.ld -static -pie --gc-sections
LDLIBS	= $(filter %$(KERNEL_SUFFIX).a, $(shell find $(LIBS_DIR)/ -maxdepth 1 -type f -name '*.a'))

###############################################################################

all:			$(BOOTLOADER) $(LIBC) $(KERNEL) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
ifdef ASMDUMP
				@mkdir -p $(ASMDUMP_DIR)
endif

###############################################################################

MAKE_VOS	= + $(MAKE) --no-print-directory -f $(SRC_DIR)/$@/Makefile THIS=$(SRC_DIR)/$@

.PHONY: $(BOOTLOADER) bootloader
$(BOOTLOADER):	setup
				$(call echo_build)
				cd $(SRC_DIR)/$@ && $(MAKE) -j1 THIS=$@ BUILD_DIR=$(BUILD_DIR_ABS) KERNEL_SRC=$(SRC_DIR)/$(KERNEL)
bootloader:		$(BOOTLOADER)

.PHONY: $(LIBS) libs
$(LIBS):		setup
				$(call echo_build)
				$(MAKE_VOS) TARGET_DIR=$(LIBS_DIR)
libs:			$(LIBS)

.PHONY: $(KERNEL) kernel
$(KERNEL):		setup
				$(call echo_build)
				$(MAKE_VOS) TARGET_DIR=$(KERNEL_DIR)
kernel:			$(KERNEL)

###############################################################################

$(VOS_BIN):		$(KERNEL_BIN)
				mv $(KERNEL_BIN) $(VOS_BIN)
.PHONY: vos
vos:			$(VOS_BIN)

$(BUILD_DIR)/$(OSNAME).img:	$(EFI_BIN) $(VOS_BIN)
				dd if=/dev/zero of=$@ bs=512 count=93750
				mformat -i $@ -f 1440 ::
				mmd -i $@ ::/EFI
				mmd -i $@ ::/EFI/BOOT
				mcopy -i $@ $(VOS_BIN) :: &
				mcopy -i $@ $(EFI_BIN) ::/EFI/BOOT &
				mcopy -i $@ -s extras/ ::/extras &
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

.PHONY: clean-bootloader
clean-bootloader:	clean-img
					rm -rf $(BUILD_DIR)/$(BOOTLOADER)/bootloader

.PHONY: clean-libs
clean-libs:			clean-img
					rm -rf $(LIBS_DIR)

.PHONY: clean-vos
clean-vos:			clean-img
					cd $(BUILD_DIR) && $(RM_OBJ)
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


include Make.rules