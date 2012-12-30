SOURCES = main.cpp ft.cpp MyAL.cpp
TEMPLATE_SOURCE = 
LIBS = -lopenal $(shell sdl-config --libs) -lsndfile -lSDL_gfx
OBJECTS	=$(addprefix obj/, $(SOURCES:.cpp=.o))
TARGET = project
EXE_NAME = project
OUTPUT_DIRECTORY = bin

INCL_COMMON_FONCTION_DIR = none

INSTALL_DIR = ./bin
COMPRESSED_FILE = libsndfile


INCL = -I$(INCL_DIR)

CXX_FLAGS =-g -ansi -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -D_DEBUG_ -std=c++11
CXX = g++ $(CXX_FLAGS) -I $(INCL_COMMON_FONCTION_DIR) -c

CFLAGS = -g -ansi -Wall
CC = gcc $(CFLAGS)

LINK_CXX = g++
LINK_CC = gcc


project: $(OBJECTS) $(OUTPUT_DIRECTORY)
	$(LINK_CXX) $(OBJECTS) -o $(OUTPUT_DIRECTORY)/$(EXE_NAME) $(LIBS)

$(OUTPUT_DIRECTORY):
	mkdir -p $(OUTPUT_DIRECTORY)

launch: project
	./$(EXE_NAME)

obj/%o: src/%cpp $(TEMPLATE_SOURCE) obj
	$(CXX) -o $@ -c $<

obj:
	mkdir -p obj

clean:
	-rm -f *.o a.out *.ps \#* *~
	-rm -f *.stackdump core
	-rm -f $(EXE_NAME)
	-rm -rf $(OUTPUT_DIRECTORY)/$(EXE_NAME)
	-rm -rf obj/*.o
	-make extra_clean

install:
	-mkdir -p $(INSTALL_DIR)
	-cp $(EXE_NAME) $(INSTALL_DIR)

uninstall:
	-rm $(INSTALL_DIR)/$(EXE_NAME)

extra_all:
extra_clean:

#-----------------------------------------------------------------------


#usage

#make project_Stack
#--> construct the executable

#make clean
#--> clean the compilation file as well as the executable created


