include Make.defaults

SUBDIRS	:= efi
EFI_BIN	:= $(BUILD_DIR)/main.efi

all:		$(SUBDIRS) $(BUILD_DIR)/$(OSNAME).img

.PHONY: setup
setup:	
			@mkdir -p $(BUILD_DIR)
			@mkdir -p $(OBJ_DIR)

.PHONY: $(SUBDIRS)
$(SUBDIRS):	
			@echo "Building $@"
			$(MAKE) -f $@/Makefile THIS=$@

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