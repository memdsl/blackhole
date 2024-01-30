COLOR_RED := $(shell echo "\033[1;31m")
COLOR_END := $(shell echo "\033[0m")

ifeq ($(wildcard .config),)
    $(warning $(COLOR_RED)Warning: .config does not exists!$(COLOR_END))
    $(warning $(COLOR_RED)To build the project, first run 'make menuconfig'.$(COLOR_END))
endif

Q            := @
KCONFIG_DIR  := $(METEOR_ENV_HOME)/core/tool/kconfig
FIXEDP_DIR   := $(METEOR_ENV_HOME)/core/tool/fixdep
KCONFIG      := $(METEOR_ENV_HOME)/core/config/Kconfig
KCONFIG_GEN  += include/generated include/config .config .config.old
SILENT       := -s

KCONFIG_CONF  := $(KCONFIG_DIR)/build/conf
KCONFIG_MCONF := $(KCONFIG_DIR)/build/mconf
FIXDEP        := $(FIXEDP_DIR)/build/fixdep

$(KCONFIG_CONF):
	$(Q)$(MAKE) $(SILENT) -C $(KCONFIG_DIR) NAME=conf

$(KCONFIG_MCONF):
	$(Q)$(MAKE) $(SILENT) -C $(KCONFIG_DIR) NAME=mconf

$(FIXDEP):
	$(Q)$(MAKE) $(SILENT) -C $(FIXEDP_DIR)

.PHONY: menuconfig defconfig help clean-config

menuconfig: $(KCONFIG_MCONF) $(KCONFIG_CONF) $(FIXDEP)
	$(Q)$(KCONFIG_MCONF) $(KCONFIG)
	$(Q)$(KCONFIG_CONF) $(SILENT) --syncconfig $(KCONFIG)

help:
	@echo "menuconfig    - Update current config utilising a menu based program"

clean-config: clean
	-@rm -rf $(KCONFIG_GEN)

define call_fixdep
	@$(FIXDEP) $(1) $(2) unused > $(1).tmp
	@mv $(1).tmp $(1)
endef
