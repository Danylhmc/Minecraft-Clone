#pragma once 
#include <memory>
#include <vector>
#include "chunk.hpp"
#include "glm/glm.hpp"
#include "perlinNoise.hpp"
#include "block.h"

class Chunk;

typedef struct RaycastResult{
  bool hit = false;
  int bx, by, bz;
  int px, py, pz;
}RaycastResult;

class World{
public:
  const static int world_size = 10;
  const int unloadRadius = 10;
  const int loadRadius = 7;
  PerlinNoise* perlinNoise;

  World(PerlinNoise* perlinNoise);
  ~World();

  void generate();
  BlockType getBlock(int x, int y, int z);
  Chunk* getChunkFromChunkPos(int worlChunkX, int worldChunkZ);
  Chunk* getChunkFromBlockPos(int worldBlockX, int worldBlockZ);
  RaycastResult raycast(glm::vec3 origin, glm::vec3 direction, float maxDist);
  void placeBlock(int x, int y, int z, BlockType type);
  void breakBlock(int x, int y, int z);

  void draw() const;
  void update(glm::vec3 cameraPos);
private:
  std::vector<std::unique_ptr<Chunk>> chunks;
};
