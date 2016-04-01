CXX=cl
ARCH=x64

BUILDDIR=build
RESOURCEDIR=resources
LIBDIR=lib
TARGET=game.exe

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%, $(BUILDDIR)/%, $(SRC:%.cpp=%.obj))

IMGUI_SRC=$(wildcard imgui/*.cpp)
IMGUI_OBJ=$(patsubst imgui/%, $(BUILDDIR)/%, $(IMGUI_SRC:%.cpp=%.obj))

COMMONFLAGS= -nologo -Z7
COMPILEFLAGS= -c -Iinclude -Iinclude/SDL -Isrc -W4 -D_CRT_SECURE_NO_WARNINGS
LINKCOMPILEFLAGS= -Fe$(BUILDDIR)/$(TARGET)
LINKFLAGS= -incremental:no -manifest:no -SUBSYSTEM:CONSOLE -LIBPATH:lib/$(ARCH)
LINKLIBS= OpenGL32.lib SDL2.lib SDL2main.lib

$(TARGET): $(BUILDDIR) $(RESOURCEDIR) $(OBJ) $(IMGUI_OBJ)
	$(CXX) $(OBJ) $(IMGUI_OBJ) $(COMMONFLAGS) $(LINKCOMPILEFLAGS) -link $(LINKFLAGS) $(LINKLIBS)

run:
	@cd $(BUILDDIR); ./$(TARGET)

$(BUILDDIR)/%.obj: src/%.cpp
	$(CXX) $(COMMONFLAGS) $(COMPILEFLAGS) $< -Fo$@

$(BUILDDIR)/%.obj: imgui/%.cpp
	$(CXX) $(COMMONFLAGS) $(COMPILEFLAGS) $< -Fo$@

clean:
	@# Note that we cannot use rm -r here because that will delete the contents of the symlink'd directory
	rmdir $(BUILDDIR)/$(RESOURCEDIR)
	rm -r $(BUILDDIR)

$(RESOURCEDIR): $(BUILDDIR)
	@cmd //c "IF NOT EXIST $(BUILDDIR)\$(RESOURCEDIR) mklink /D $(BUILDDIR)\$(RESOURCEDIR) ..\$(RESOURCEDIR)"

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	@cp $(LIBDIR)/$(ARCH)/SDL2.dll $(BUILDDIR)/
