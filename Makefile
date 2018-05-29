include Sources.mk

APPNAME=ModelViewer

##CPP COMPILATION
INCLUDES=$(G_INCLUDE_FLAGS)
DEFINES=$(G_COMPILER_FLAGS)
OPT_FLAGS=$(G_OPT_FLAGS)
WARN_FLAGS=$(G_WARN_FLAGS)
EXTRA_FLAGS=$(G_EXTRA_FLAGS)
FRAMEWORKS=$(G_FRAMEWORKS)
LIB_FLAGS=$(G_LIB_FLAGS)

CC:=gcc
CCC:=g++
#AR:
CURR:=$(shell pwd)
OUTDIR:=build
OBJ_OUT=$(OUTDIR)/objs

SOURCES = $(SRC)
SOURCE_CPP = $(filter %.cpp, $(SOURCES))
SOURCE_C = $(filter %.c, $(SOURCES))
SOURCE_FILES = $(SOURCE_CPP:.cpp=.o) $(SOURCE_C:.c=.o)
SOURCE_OBJS = $(SOURCE_FILES)

SRC_OBJS = $(addprefix $(OBJ_OUT)/, $(SOURCE_OBJS))
OBJS = $(SRC_OBJS)

##TARGETS
all: $(OBJS)
	$(CCC) $(OPT_FLAGS) \
	$(DEFINES) $(INCLUDES) $(WARN_FLAGS) \
	$(FRAMEWORKS) \
	-o $(OUTDIR)/$(APPNAME) \
	$(OBJS) \
	$(LIB_FLAGS)

$(OBJ_OUT)/%.o: %.c
	mkdir -p $(dir $(@))
	$(CC) $(OPT_FLAGS) $(DEFINE) $(INCLUDES) $(COMPILE_FLAGS) $(EXTRA_FLAGS) -c $< -o $(@)

$(OBJ_OUT)/%.o: %.cpp
	mkdir -p $(dir $(@))
	$(CCC) -stdlib=libc++ -std=c++11 $(OPT_FLAGS) $(DEFINES) $(INCLUDES) $(COMPILE_FLAGS) $(EXTRA_FLAGS) -c $< -o $(@)

clean:
	rm -rf $(OBJS) $(OUTDIR)/