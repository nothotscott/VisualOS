include Make.defaults

SUBDIRS	:= kernel
EFI_BIN	:= $(BUILD_DIR)/gnu-efi/bootloader/main.efi

all:		$(SUBDIRS) $(BUILD_DIR)/$(OSNAME).img

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
			mcopy -i $@ efi/startup.nsh ::


.PHONY: clean
clean:		
			find $(BUILD_DIR) -type f -name '*.*' -delete

.PHONY: superclean
superclean:	
			rm -rf $(BUILD_DIR)