TOTAL_ALLOCATED_MEM_MB := 112
TOTAL_ALLOCATED_MEM_BYTES := $(shell echo $(TOTAL_ALLOCATED_MEM_MB)*1048576 | bc)
USE_WASM := 0

VPATH := ./

G_INCLUDE_FLAGS:=

G_COMPILER_FLAGS:=#-D compiler flags

G_LIB_FLAGS:=#-L Lib flags

G_EXTRA_FLAGS:=#-fno-stack-protector

G_WARN_FLAGS:=#-Wnotrigraphs -Wno-sign-compare

G_OPT_FLAGS := -O3 \
	--llvm-lto 3 \
	-emit-llvm

G_EMSCRIPT_FLAGS := --bind \
	-s TOTAL_MEMORY=$(TOTAL_ALLOCATED_MEM_BYTES) \
	-s USE_ZLIB=1 \
	-s LZ4=1 \
	-s MEMFS_APPEND_TO_TYPED_ARRAYS=1 \
	-s AGGRESSIVE_VARIABLE_ELIMINATION=1 \
	-s DISABLE_EXCEPTION_CATCHING=0 

ifeq ($(USE_WASM), 1)
	G_EMSCRIPT_FLAGS += -s WASM=1 
	#-s "BINARYEN_TRAP_MODE='clamp'"
endif

SRC += main.cpp