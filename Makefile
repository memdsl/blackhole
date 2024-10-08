ifeq ($(shell command -v kconfig-mconf >/dev/null 2>&1 && echo yes || echo no), no)
    $(error $(COLOR_RED)[error] kconfig-mconf is not installed, please install [kconfig-frontends]$(COLOR_RED))
endif

CONFIG_DIR = $(BLACKHOLE_HOME)/config
CONFIG_NEW = $(CONFIG_DIR)/.config

BUILD_DIR = $(BLACKHOLE_HOME)/build
BUILD_BIN = $(BUILD_DIR)/blackhole

CXX    = g++
CXX_LD = $(CXX)

-include $(OBJS:.o=.d)
-include $(CONFIG_DIR)/autoconf.m

CXX_CFLAGS  = -Wall \
              -MMD  \
              $(if $(CONFIG_BASE_COMP_DEBUG),-g,)
CXX_LDFLAGS = -lfmt

INCS_DIR = $(shell find include -type d) \
           config
INCS     = $(addprefix -I, $(INCS_DIR))

SRCS = $(shell find src -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
LIBS =

ARGS_BCH ?= ""
ARGS_IMG ?= ""
ARGS_LOG ?= $(BUILD_DIR)/log.txt
ARGS_REF ?= ""
ARGS_OTH ?=
ARGS      = --bch $(ARGS_BCH) \
            --img $(ARGS_IMG) \
            --log $(ARGS_LOG) \
            --ref $(ARGS_REF) \
                  $(ARGS_OTH)

$(BUILD_DIR)/%.o: %.cpp $(CONFIG_NEW)
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXX_CFLAGS) $(INCS) -c -o $@ $<

$(BUILD_BIN): $(OBJS)
	@echo + LD $@
	@$(CXX_LD) $(OBJS) $(CXX_LDFLAGS) $(LIBS) -o $@

.PHONY: config run clean

config:
	cd config && kconfig-mconf Kconfig && ../script/gen_autoconf.sh
run: $(BUILD_BIN)
	$(BUILD_BIN) $(ARGS)
gdb: $(BUILD_BIN)
	gdb --args $(BUILD_BIN) $(ARGS)
clean:
	rm -rf $(BUILD_DIR)
