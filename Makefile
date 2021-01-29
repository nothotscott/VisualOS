THIS		:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include		Make.defaults

TARGET_DIR	= $(BUILD_DIR)

EFI_BIN			:= $(BUILD_DIR)/gnu-efi/bootloader/BOOTX64.efi
STARTUP_SCRIPT	:= $(SRC_DIR)/$(BOOTLOADER)/startup.nsh
KERNEL_BIN		:= $(KERNEL_DIR)/kernel.elf
VOS_BIN			:= $(BUILD_DIR)/kernel.elf

LDFLAGS	:= -T $(SRC_DIR)/vos.ld -static -Bsymbolic -nostdlib
LDLIBS	= $(wildcard $(LIBC_DIR)/lib/*.a) $(wildcard $(LIB_DIR)/*.a)

###############################################################################

all:			$(BOOTLOADER) $(LIBC) $(LIBRARY) $(KERNEL) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
				@mkdir -p $(BUILD_DIR)
ifdef ASMDUMP
				@mkdir -p $(ASMDUMP_DIR)
endif

###############################################################################

MAKE_VOS	= + $(MAKE) --no-print-directory -f $(SRC_DIR)/$@/Makefile THIS=$(SRC_DIR)/$@

.PHONY: $(BOOTLOADER)
$(BOOTLOADER):	setup
				$(call echo_build)
				cd $(SRC_DIR)/$@ && $(MAKE) -j1 THIS=$@ BUILD_DIR=$(BUILD_DIR_ABS) KERNEL_SRC=$(SRC_DIR)/$(KERNEL)

.PHONY: $(LIBC)
$(LIBC):		setup
				$(call echo_build)
				cd $(SRC_DIR)/$@ && $(MAKE) THIS=$@ BUILD_DIR=$(BUILD_DIR_ABS)

.PHONY: $(LIBRARY)
$(LIBRARY):		setup
				$(call echo_build)
				$(MAKE_VOS) TARGET_DIR=$(LIB_DIR)

.PHONY: $(KERNEL)
$(KERNEL):		setup $(LIBRARY)
				$(call echo_build)
				$(MAKE_VOS) TARGET_DIR=$(KERNEL_DIR)

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
				mcopy -i $@ $(STARTUP_SCRIPT) :: &
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

.PHONY: clean-gnuefi
clean-gnuefi:	clean-img
				rm -rf $(BUILD_DIR)/gnu-efi/bootloader

.PHONY: clean-musl
clean-musl:		clean-img
				rm -rf $(BUILD_DIR)/musl

.PHONY: clean-vos
clean-vos:		clean-img
				cd $(BUILD_DIR) && $(RM_OBJ)
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


include Make.rules