ifeq ($(OS),Windows_NT)
	CXX=cl
else
	CXX=g++
endif

ARCH=x64

BUILDDIR=build
RESOURCEDIR=resources
LIBDIR=lib

ifeq ($(OS),Windows_NT)
	TARGET=lostLuggage.exe
else
	TARGET=lostLuggage
endif

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%, $(BUILDDIR)/%, $(SRC:%.cpp=%.obj))

IMGUI_SRC=$(wildcard imgui/*.cpp)
IMGUI_OBJ=$(patsubst imgui/%, $(BUILDDIR)/%, $(IMGUI_SRC:%.cpp=%.obj))

ifeq ($(OS),Windows_NT)
	COMMONFLAGS= -nologo -Z7
	LINKFLAGS= -incremental:no -manifest:no -SUBSYSTEM:CONSOLE -LIBPATH:lib/$(ARCH)
	LINKLIBS= OpenGL32.lib SDL2.lib SDL2main.lib
	LINKCOMPILEFLAGS= -Fe$(BUILDDIR)/$(TARGET) -link $(LINKFLAGS) $(LINKLIBS)
	COMPILEFLAGS= -c -Iinclude -Iinclude/SDL -Isrc -W4 -D_CRT_SECURE_NO_WARNINGS -Wall -Fo
else
	COMMONFLAGS= -g --std=c++11
	LINKCOMPILEFLAGS= -o $(BUILDDIR)/$(TARGET) -lGL -ldl `sdl2-config --libs`
	COMPILEFLAGS= -c `sdl2-config --cflags` -Iinclude -Isrc -D_CRT_SECURE_NO_WARNINGS -Wall -o
endif

ifeq ($(OS),Windows_NT)
	RESOURCEDIRCMD=@cmd //c "IF NOT EXIST $(BUILDDIR)\$(RESOURCEDIR) mklink /D $(BUILDDIR)\$(RESOURCEDIR) ..\$(RESOURCEDIR)"
	BUILDLIBDIRCMD=@cp $(LIBDIR)/$(ARCH)/SDL2.dll $(BUILDDIR)/
	RESOURCECLEANCMD=-rmdir $(BUILDDIR)/$(RESOURCEDIR)
else
	RESOURCEDIRCMD=ln -s ../$(RESOURCEDIR) $(BUILDDIR)/$(RESOURCEDIR)
	BUILDLIBDIRCMD=
	RESOURCECLEANCMD=-rm $(BUILDDIR)/$(RESOURCEDIR)
endif



$(TARGET): $(BUILDDIR) $(BUILDDIR)/$(RESOURCEDIR) $(OBJ) $(IMGUI_OBJ)
	$(CXX) $(OBJ) $(IMGUI_OBJ) $(COMMONFLAGS) $(LINKCOMPILEFLAGS)

run:
	@cd $(BUILDDIR); ./$(TARGET)

$(BUILDDIR)/%.obj: src/%.cpp
	$(CXX) $< $(COMMONFLAGS) $(COMPILEFLAGS)$@

$(BUILDDIR)/%.obj: imgui/%.cpp
	$(CXX) $< $(COMMONFLAGS) $(COMPILEFLAGS)$@

clean:
	@# Note that we cannot use rm -r here because that will delete the contents of the symlink'd directory
	$(RESOURCECLEANCMD)
	rm -r $(BUILDDIR)

$(BUILDDIR)/$(RESOURCEDIR): $(BUILDDIR)
	$(RESOURCEDIRCMD)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	$(BUILDLIBDIRCMD)
