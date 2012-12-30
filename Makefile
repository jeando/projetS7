SOURCES = main.cpp ft.cpp
TEMPLATE_SOURCE = 
LIBS = -lopenal $(shell sdl-config --libs) -lsndfile -lSDL_gfx
OBJECTS	=	$(SOURCES:.cpp=.o)
TARGET = project


INCL_COMMON_FONCTION_DIR = none

INSTALL_DIR = ./bin
COMPRESSED_FILE = libsndfile

EXE_NAME=project

INCL = -I$(INCL_DIR)

CXX_FLAGS =-g -ansi -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -D_DEBUG_ -std=c++11
CXX = g++ $(CXX_FLAGS) -I $(INCL_COMMON_FONCTION_DIR) -c

CFLAGS = -g -ansi -Wall
CC = gcc $(CFLAGS)

LINK_CXX = g++
LINK_CC = gcc



launch: project
	./$(EXE_NAME)

project: $(OBJECTS)
	$(LINK_CXX) $(OBJECTS) -o $(EXE_NAME) $(LIBS)

%o: %cpp $(TEMPLATE_SOURCE)
	$(CXX) -o $@ -c $<

clean:
	-rm -f *.o a.out *.ps \#* *~
	-rm -f *.stackdump core
	-rm -f $(EXE_NAME)
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


