ifeq ($(shell command -v kconfig-mconf >/dev/null 2>&1 && echo yes || echo no), no)
    $(error $(COLOR_RED)[error] kconfig-mconf is not installed, please install [kconfig-frontends]$(COLOR_RED))
endif

BUILD_DIR = $(BLACKHOLE_HOME)/build
BUILD_BIN = $(BUILD_DIR)/blackhole

CXX    = g++
CXX_LD = $(CXX)

CXX_CFLAGS  = -Wall
CXX_LDFLAGS = -lfmt

INCS_DIR = $(shell find include -type d)
INCS     = $(addprefix -I, $(INCS_DIR))

SRCS = $(shell find src -name "*.cpp")
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
LIBS =

-include $(OBJS:.o=.d)

$(BUILD_DIR)/%.o: %.cpp
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
	$(BUILD_BIN)
clean:
	rm -rf $(BUILD_DIR)
