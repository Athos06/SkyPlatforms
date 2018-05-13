# --------------------------------------------------------------------
# Makefile Genérico :: Módulo 2. Curso Experto Desarrollo Videojuegos
# Carlos González Morcillo     Escuela Superior de Informática (UCLM)
# --------------------------------------------------------------------
EXEC := SkyPlatforms

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/
DIRCEGUI := /usr/local/include/CEGUI/
DIROGRE := RendererModules/Ogre/
DIRSDL := SDL/

CXX := g++


# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -I $(DIRHEA)$(DIRSDL) -I $(DIRCEGUI) -I $(DIRCEGUI)$(DIROGRE) -Wall `pkg-config --cflags OGRE --libs sdl` `pkg-config --cflags bullet`

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE --libs sdl` `pkg-config --libs-only-l bullet`
LDLIBS := `pkg-config --libs-only-l OGRE --libs sdl` `pkg-config --libs-only-l bullet` -lOIS -lGL -lstdc++ -lCEGUIBase -lCEGUIOgreRenderer -lCEGUIFalagardWRBase -lOgreMain -lOgreTerrain  -lSDL_image -lSDL_mixer 

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O3 -D_RELEASE	
else 
	CXXFLAGS += -O -D_DEBUG
endif

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDLIBS)

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~ 

edit:
	emacs $(wildcard $(DIRSRC)*.cpp) $(wildcard $(DIRHEA)*.h) &
