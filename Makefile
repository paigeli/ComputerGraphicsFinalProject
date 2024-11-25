################################################################################
## OS
################################################################################
# OS = LINUX
OS = OSX

################################################################################
## GCC
################################################################################
CC = g++ -std=c++14
OPTS = -O3
#OPTS = -g
FLAGS = #-Wall -Werror
ifeq "$(OS)" "LINUX"
  DEFS = -DLINUX
else
  ifeq "$(OS)" "OSX"
  DEFS = -DOSX
endif
endif

################################################################################
## GL and QT
################################################################################
# Open gl
ifeq "$(OS)" "LINUX"
  # GL_LIBS = -lglut -lGL
  GL_LIBS = -lglfw -lGL
else
  ifeq "$(OS)" "OSX"
  # GL_LIBS = -framework GLUT -framework OpenGL
  GL_INCL = -I./SOIL2-master/src
  GL_LIBS = -lglfw -framework OpenGL -framework Cocoa ./SOIL2-master/lib/macosx/libsoil2-debug.a
endif
endif

################################################################################
## Rules
################################################################################
INCL = $(GL_INCL)
LIBS = $(GL_LIBS)

OBJS = \
			 CompileShaders.o \
       ParticleSystem.o \
       Scene.o  \
       Forces/Attraction.o \
       Forces/ConstantForce.o \
       Generators/UniformGenerator.o \
       Generators/NormalGenerator.o \
       Render/RayTracing.o \
       Render/Rasterization.o \
       Models/Camera.o \
       Models/Light.o \
       Models/Plane.o \
       Models/Sphere.o \
       Models/Material.o \
       Models/Mesh.o \
       ObjParser.o \
       main.o

EXECUTABLE = spiderling

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(OBJMOC)
	$(CC) $(OPTS) $(FLAGS) $(DEFS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) Dependencies $(OBJS)

.cpp.o:
	$(CC) $(OPTS) $(DEFS) -MMD $(INCL) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d

-include Dependencies
