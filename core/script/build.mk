ifeq ($(SHARE), 1)
    SO       = -so
    CFLAGS  += -fPIC
    LDFLAGS += -rdynamic -shared -fPIC
endif

CURRT_DIR  = $(shell pwd)
BUILD_DIR  = $(CURRT_DIR)/build
OBJ_DIR    = $(BUILD_DIR)/obj-$(NAME)$(SO)
INC_DIR   := $(CURRT_DIR)/include $(INC_DIR)

BIN = $(BUILD_DIR)/$(NAME)$(SO)

ifeq ($(CC), clang)
    CXX := clang++
else
    CXX := g++
endif
LD := $(CXX)

INCFLAGS  = $(addprefix -I, $(INC_DIR))
CFLAGS   := -O2 -MMD -Wall -Werror -Wno-unused-result -Wno-maybe-uninitialized \
            $(INCFLAGS) \
            $(CFLAGS)
LDFLAGS  := -O2 $(LDFLAGS)

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o) $(CXXSRC:%.cc=$(OBJ_DIR)/%.o)

-include $(OBJS:.o=.d)

$(OBJ_DIR)/%.o: %.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -g -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)

$(OBJ_DIR)/%.o: %.cc
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)

$(BIN): $(OBJS) $(ARCHIVES)
	@echo + LD $@
	@$(LD) -o $@ $(OBJS) $(LDFLAGS) $(ARCHIVES) $(LIBS)

.PHONY: app clean

run: $(BIN)

clean:
	-rm -rf $(BUILD_DIR)
