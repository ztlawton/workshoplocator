#source file search paths
VPATH = ./src 

	# compiler include search paths
INCS = -I./include 

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs

	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static -lComctl32

# folders
ODIR = ./.vscode/obj

DEPS = ../src/workshop.h

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_starter = \
	$(ODIR)/main.o 

### COMPILE

$(ODIR)/%.o: %.cpp  $(DEPS)
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

starter: $(OBJ_starter)
	g++ $(OPT) \
	-o bin/starter.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

