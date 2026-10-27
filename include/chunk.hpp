#pragma once 

#include <glad/glad.h>

#include "block.h"
#include "perlinNoise.hpp"

class World;

#include <vector>

enum FaceSide{
  TOP    = 0,
  BOTTOM = 1,
  RIGHT  = 2,
  LEFT   = 3,
  FRONT  = 4,
  BACK   = 5
};

static float cubeFaces[6][48]{
  //TOP
  {
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 
     1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
  },
  //BOTTOM
  {
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f

  },
  //RIGHT
  {
    1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
  },
  //LEFT
  {
    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
  },
  //FRONT
  {
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f
  },
  //BACK
  {
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  }
};

struct position{
  int x = 0;
  int y = 0;
  int z = 0;
};

class World;

class Chunk{
public:
  const static int chunk_size = 16;
  const static int chunk_height = 32;
  position pos;
  World* world;
  int vertexCount = 0;
  Chunk(PerlinNoise* perlinNoise, World* world, int chunkX, int chunkZ);
  ~Chunk();

  bool isTransparent(int x, int y, int z);
  BlockType getBlock(int x, int y, int z);
  void setBlock(int x, int y, int z, BlockType type);

  void setMesh();
  void rebuildMesh();
  void addFace(std::vector<float>* vertices, int x, int y, int z, FaceSide side, int type);
  void draw();
private:
  unsigned int VAO = 0;
  unsigned int VBO = 0;
  BlockType blocks[chunk_size][chunk_height][chunk_size];
};
