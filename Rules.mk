define mkdir_subtarget
	@mkdir -p $(dir $@)
endef

.PRECIOUS:	$(TARGET_DIR)/%.o $(TARGET_DIR)/%_c.o $(TARGET_DIR)/%_cpp.o $(TARGET_DIR)/%_asm.o $(TARGET_DIR)/%_s.o

MACROS	= $(if $(MACRO_FILENAME_ENABLE),-D__FILENAME__="\"$<\"")\
		$(if $(MACRO_MODULENAME_ENABLE),-D__MODULE__="\"$(notdir $(basename $<))\"")\
		$(if $(MACRO_GITREVISION_ENABLE),-D__GIT_REVISION__=\"$(shell git rev-parse HEAD)\")\

$(TARGET_DIR)/%.o $(TARGET_DIR)/%_c.o:		$(THIS)/%.c
											$(call mkdir_subtarget)
											$(CC) $(strip $(CFLAGS) $(MACROS)) -c -MD $< -o $@

$(TARGET_DIR)/%.o $(TARGET_DIR)/%_cpp.o:	$(THIS)/%.cpp
											$(call mkdir_subtarget)
											$(CXX) $(CXXFLAGS) -c -MD $< -o $@

$(TARGET_DIR)/%.o $(TARGET_DIR)/%_asm.o:	$(THIS)/%.asm
											$(call mkdir_subtarget)
											$(AS) $(ASFLAGS) -i $(dir $<) $< -o $@ -MD $(basename $@).d

$(TARGET_DIR)/%.o $(TARGET_DIR)/%_s.o:		$(THIS)/%.s
											$(call mkdir_subtarget)
											$(CAS) $(CASFLAGS) -c -MD $< -o $@

#######################################

.PRECIOUS:	$(TARGET_DIR)/%.d $(TARGET_DIR)/%_c.d $(TARGET_DIR)/%_cpp.d $(TARGET_DIR)/%_asm.d
include $(shell find $(TARGET_DIR)/ -type f -name '*.d')

#######################################
LDLIBS_FILTERED	= $(filter-out $^, $(LDLIBS))

$(TARGET_DIR)/%.elf:		$(TARGET_DIR)/%.ro
							$(LD) $(LDFLAGS) $^ $(LDLIBS_FILTERED) -o $@
#							$(OBJCOPY) $(OBJCOPYFLAGS) --strip-debug $@ $(addsuffix _release.elf, $(basename $@))

# Multiple object
$(TARGET_DIR)/%.mo:			# No prerequisites
							$(LD) $(LDFLAGS) $(LDLIBS) -o $@

# Relocatable object
$(TARGET_DIR)/%.ro:			# No prerequisites
							$(LD) $(LDFLAGS) -r $(LDLIBS) -o $@

# May have to be $(CXX)
$(TARGET_DIR)/%.so:			$(TARGET_DIR)/%.o
							$(LD) $(LDFLAGS) $^ $(LDLIBS_FILTERED) -o $@

# May have to be $(CXX)
$(TARGET_DIR)/%.so:			# No prerequisites
							$(LD) $(LDFLAGS) $(LDLIBS) -o $@

$(TARGET_DIR)/%.a:			$(TARGET_DIR)/%.o
							$(AR) $(ARFLAGS) $@ $^ $(filter-out $^, $(ARLIBS))

$(TARGET_DIR)/%.a:			# No prerequisites
							$(AR) $(ARFLAGS) $@ $(ARLIBS)

$(TARGET_DIR)/%.efi:		$(TARGET_DIR)/%.so
							$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc $(OBJCOPYFLAGS) $< $@

# Convert to assembly
$(ASMDUMP_DIR)/%.s:	$(THIS)/%.c
					$(CC) $(CFLAGS) -S -masm=intel -fverbose-asm $< -o $@

$(ASMDUMP_DIR)/%.s:	$(THIS)/%.cpp
					$(CXX) $(CXXFLAGS) -S -masm=intel -fverbose-asm $< -o $@
