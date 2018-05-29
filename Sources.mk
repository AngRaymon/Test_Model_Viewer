
VPATH := src

G_INCLUDE_FLAGS:= -I/Library/Frameworks/SDL2.framework/Headers \
	-I/usr/include \
	-Isrc/include \
	-Isrc/renderer \
	-Isrc/renderer/shaders \
	-Isrc/model \
	-Isrc

G_COMPILER_FLAGS:=#-D compiler flags

G_LIB_FLAGS:= -Lsrc/lib \
	-ltinyxml2

G_FRAMEWORKS:= -framework SDL2 \
	-framework GLUT \
	-framework OpenGL \
	-framework Cocoa

G_EXTRA_FLAGS:=#-fno-stack-protector

G_WARN_FLAGS:=#-Wnotrigraphs -Wno-sign-compare

G_OPT_FLAGS := -O0 -g3 

SRC += main.cpp \
	renderer/Renderer_GL.cpp \
	renderer/VAO.cpp \
	renderer/shaders/ColoredVertShader.cpp \
	renderer/shaders/NormalShader.cpp \
	model/ColladaLoader.cpp \
	model/Model.cpp