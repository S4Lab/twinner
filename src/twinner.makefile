ifdef PIN_ROOT
CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
else
CONFIG_ROOT := ../Config
endif

KIT := 1
COMPILER := gcc
TARGET_OS := linux
include $(CONFIG_ROOT)/unix.vars
UNIX_CONFIG_MAKEFILE := $(shell find $(CONFIG_ROOT) -name makefile.unix\* | sort | head -n1 | xargs basename)
include $(CONFIG_ROOT)/$(UNIX_CONFIG_MAKEFILE)

APP_CC := $(CC)
APP_CXX := $(CXX)

# Define the test application
TESTAPP := $(TOOLS_ROOT)/Utils/$(OBJDIR)cp-pin$(EXE_SUFFIX)
HELLO_APP := $(TOOLS_ROOT)/Utils/$(OBJDIR)hello$(EXE_SUFFIX)
HELLO_APP_DWARF4 := $(TOOLS_ROOT)/Utils/$(OBJDIR)hello_dwarf4$(EXE_SUFFIX)

# Define the HW check utilities
CHECKAVX := $(TOOLS_ROOT)/Utils/$(OBJDIR)avx_check$(EXE_SUFFIX)
CHECKAVX2 := $(TOOLS_ROOT)/Utils/$(OBJDIR)avx2_check$(EXE_SUFFIX)
CHECKTSX := $(TOOLS_ROOT)/Utils/$(OBJDIR)tsx_check$(EXE_SUFFIX)

# Define the thread utilities library
THREADLIB := $(TOOLS_ROOT)/Utils/$(OBJDIR)threadlib$(OBJ_SUFFIX)

# Define the regval utilities library
REGVALLIB := $(TOOLS_ROOT)/Utils/$(OBJDIR)regvallib$(LIB_SUFFIX)

# Define the controller utilities library
CONTROLLERLIB := $(TOOLS_ROOT)/InstLib/$(OBJDIR)controller$(LIB_SUFFIX)

# Common object files
SUPPORTS_AVX_OBJ := $(TOOLS_ROOT)/Utils/$(OBJDIR)supports_avx$(OBJ_SUFFIX)
SUPPORTS_AVX2_OBJ := $(TOOLS_ROOT)/Utils/$(OBJDIR)supports_avx2$(OBJ_SUFFIX)

# Python comparison script
PYCOMPARE := $(PYTHON) $(TOOLS_ROOT)/Utils/compare.py

TEST_TOOL_ROOTS :=
TEST_ROOTS :=
SANITY_SUBSET :=
TOOL_ROOTS :=
SA_TOOL_ROOTS :=
APP_ROOTS :=
OBJECT_ROOTS :=
DLL_ROOTS :=
LIB_ROOTS :=









TOOL_CXXFLAGS += -I.
APP_CXXFLAGS += -I.

MY_FILTER = $(foreach p,$(2),$(if $(findstring $(1),$(p)),$(p),))

TWINNER_SRC := $(shell find edu -name \*.cpp; find edu -type d -exec mkdir -p $(OBJDIR)'{}' \;)
TWINNER_OBJ := $(patsubst %.cpp,$(OBJDIR)%$(OBJ_SUFFIX), $(TWINNER_SRC))
TWINNER_SUB_TRACE_OBJ := $(call MY_FILTER,/trace/,$(TWINNER_OBJ)) $(call MY_FILTER,/util/,$(TWINNER_OBJ))
TWINNER_SUB_ENGINE_OBJ := $(call MY_FILTER,/engine/,$(TWINNER_OBJ)) $(call MY_FILTER,/cli/,$(TWINNER_OBJ))
TWINNER_DEP := $(TWINNER_OBJ:$(OBJ_SUFFIX)=.d)

TWINNER_LIBS := -L/usr/local/lib -lcvc4 -lcln -lgmp

$(OBJDIR)%.d: %.cpp
	./depends.sh $@ $*.cpp $(TOOL_CXXFLAGS) > $@

include $(TWINNER_DEP)


$(OBJDIR)Trace$(LIB_SUFFIX): $(TWINNER_SUB_TRACE_OBJ)
	rm -f $@
	$(ARCHIVER)$@ $(TWINNER_SUB_TRACE_OBJ)

$(OBJDIR)Twinner$(EXE_SUFFIX): $(TWINNER_SUB_ENGINE_OBJ) $(OBJDIR)Trace$(LIB_SUFFIX)
	$(LINKER) $(COMP_EXE)$@ $(TWINNER_SUB_ENGINE_OBJ) $(OBJDIR)Trace$(LIB_SUFFIX) $(APP_LDFLAGS) $(APP_LIBS) $(TWINNER_LIBS)

twinner: $(OBJDIR)Twinner$(EXE_SUFFIX)

.PHONY: twinner

include $(TOOLS_ROOT)/Config/makefile.default.rules

