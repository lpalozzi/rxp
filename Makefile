TARGET = rxp

CXXFLAGS = -g -I/usr/include/bullet -I/usr/include/SDL2 -D_REENTRANT

OBJS = \
	src/Shader.o \
	src/Mesh.o \
	src/SceneNode.o \
	src/Renderer.o \
	src/Actor.o \
	src/Table.o \
	src/Flipper.o \
	src/Pinball.o \
	src/StaticMesh.o \
	src/StaticMeshNode.o \
	src/PhysicalMeshNode.o \
	src/PhysicalActor.o \
	src/SceneManager.o \
	src/SceneLoader.o \
	src/Game.o \
	src/rxp.o

LIBS = -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lGL -lBulletDynamics -lBulletCollision -lLinearMath


$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.o : %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<
   
all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
