#include "world.hpp"
#include <memory>
#include <cmath>
#include <vector>
#include <algorithm>

World::World(PerlinNoise* perlinNoise)
:perlinNoise(perlinNoise){
  generate();
}

World::~World(){
}

void World::generate(){
  chunks.clear();
  chunks.reserve(world_size *world_size);
  for(int x = 0; x < world_size; x++){
    for(int z = 0; z < world_size; z++){
      chunks.push_back(std::make_unique<Chunk>(perlinNoise, this, x * Chunk::chunk_size, z * Chunk::chunk_size));
    }
  }

  for(auto& chunk : chunks){
    chunk->setMesh();
  }
}

BlockType World::getBlock(int worldX , int y, int worldZ){
  if(y >= Chunk::chunk_height || y < 0) return AIR;

  int chunkX = (int)std::floor((float)worldX / Chunk::chunk_size);
  int chunkZ = (int)std::floor((float)worldZ / Chunk::chunk_size);

  int localX = worldX - chunkX * Chunk::chunk_size;
  int localZ = worldZ - chunkZ * Chunk::chunk_size;

  for(auto& chunk : chunks){
    if(chunk->pos.x / Chunk::chunk_size == chunkX &&
       chunk->pos.z / Chunk::chunk_size == chunkZ){
      return chunk->getBlock(localX, y, localZ);
    }
  }

  return AIR;
}

Chunk* World::getChunkFromChunkPos(int worldChunkX, int worldChunkZ){
  for(auto& chunk : chunks){
    if(chunk->pos.x == worldChunkX && chunk->pos.z == worldChunkZ ) return chunk.get();
  }
  return nullptr;
}

Chunk* World::getChunkFromBlockPos(int worldBlockX, int worldBlockZ){
  int chunkX = (int)std::floor((float)worldBlockX / Chunk::chunk_size) * Chunk::chunk_size;
  int chunkZ = (int)std::floor((float)worldBlockZ / Chunk::chunk_size) * Chunk::chunk_size;
  return getChunkFromChunkPos(chunkX, chunkZ);
}

RaycastResult World::raycast(glm::vec3 origin, glm::vec3 direction, float maxDist){
  RaycastResult result = {false, 0, 0, 0, 0, 0, 0};

  float stepValue = 0.05f;
  glm::vec3 prev = origin;
  for(float t = 0; t < maxDist; t+= stepValue){
    glm::vec3 pos = origin + direction * t;

    int bx = (int)std::floor(pos.x);
    int by = (int)std::floor(pos.y);
    int bz = (int)std::floor(pos.z);

    if(getBlock(bx, by, bz) != AIR){
      result.hit = true;

      result.bx = bx;
      result.by = by;
      result.bz = bz;

      result.px = (int)std::floor(prev.x);
      result.py = (int)std::floor(prev.y);
      result.pz = (int)std::floor(prev.z);

      return result;
    }

    prev = pos;
  }
  return result;
}

void World::placeBlock(int x, int y, int z, BlockType type){
  if(getBlock(x, y, z) != AIR) return;

  int chunkX = (int)std::floor((float)x / Chunk::chunk_size) * Chunk::chunk_size;
  int chunkZ = (int)std::floor((float)z / Chunk::chunk_size) * Chunk::chunk_size;

  Chunk* chunk = getChunkFromChunkPos(chunkX, chunkZ);
  if(!chunk) return;

  int localX = x - chunkX;
  int localZ = z - chunkZ;
  chunk->setBlock(localX, y, localZ, type);

  // Reconstruire les chunks voisins si on est sur un bord
  if(localX == 0){
    Chunk* n = getChunkFromChunkPos(chunkX - Chunk::chunk_size, chunkZ);
    if(n) n->rebuildMesh();
  }
  if(localX == Chunk::chunk_size - 1){
    Chunk* n = getChunkFromChunkPos(chunkX + Chunk::chunk_size, chunkZ);
    if(n) n->rebuildMesh();
  }
  if(localZ == 0){
    Chunk* n = getChunkFromChunkPos(chunkX, chunkZ - Chunk::chunk_size);
    if(n) n->rebuildMesh();
  }
  if(localZ == Chunk::chunk_size - 1){
    Chunk* n = getChunkFromChunkPos(chunkX, chunkZ + Chunk::chunk_size);
    if(n) n->rebuildMesh();
  }

  chunk->rebuildMesh();
}

void World::breakBlock(int x, int y, int z){
  if(getBlock(x, y, z) == AIR) return;

  int chunkX = (int)std::floor((float)x / Chunk::chunk_size) * Chunk::chunk_size;
  int chunkZ = (int)std::floor((float)z / Chunk::chunk_size) * Chunk::chunk_size;

  Chunk* chunk = getChunkFromChunkPos(chunkX, chunkZ);
  if(!chunk) return;

  int localX = x - chunkX;
  int localZ = z - chunkZ;
  chunk->setBlock(localX, y, localZ, AIR);

  // Reconstruire les chunks voisins si on est sur un bord
  if(localX == 0){
    Chunk* n = getChunkFromChunkPos(chunkX - Chunk::chunk_size, chunkZ);
    if(n) n->rebuildMesh();
  }
  if(localX == Chunk::chunk_size - 1){
    Chunk* n = getChunkFromChunkPos(chunkX + Chunk::chunk_size, chunkZ);
    if(n) n->rebuildMesh();
  }
  if(localZ == 0){
    Chunk* n = getChunkFromChunkPos(chunkX, chunkZ - Chunk::chunk_size);
    if(n) n->rebuildMesh();
  }
  if(localZ == Chunk::chunk_size - 1){
    Chunk* n = getChunkFromChunkPos(chunkX, chunkZ + Chunk::chunk_size);
    if(n) n->rebuildMesh();
  }

  chunk->rebuildMesh();
}

void World::draw() const{
  for(auto& chunk : chunks){
    chunk->draw();
  }
}

void World::update(glm::vec3 cameraPos){
  int playerChunkX = (int)std::floor(cameraPos.x / Chunk::chunk_size);
  int playerChunkZ = (int)std::floor(cameraPos.z / Chunk::chunk_size);

  const int s = Chunk::chunk_size;
  std::vector<std::pair<int, int>> chunksToRebuild;

  auto addPosition = [&](int worldX, int worldZ) {
    chunksToRebuild.emplace_back(worldX, worldZ);
  };

  auto addNeighbours = [&](int worldX, int worldZ) {
    addPosition(worldX + s, worldZ);
    addPosition(worldX - s, worldZ);
    addPosition(worldX, worldZ + s);
    addPosition(worldX, worldZ - s);
  };

  for (auto it = chunks.begin(); it != chunks.end();) {
    Chunk* chunk = it->get();

    int dx = chunk->pos.x / s - playerChunkX;
    int dz = chunk->pos.z / s - playerChunkZ;

    if (std::abs(dx) > unloadRadius || std::abs(dz) > unloadRadius) {
      addNeighbours(chunk->pos.x, chunk->pos.z);
      it = chunks.erase(it);
    } else {
      ++it;
    }
  }

  for (int dx = -loadRadius; dx <= loadRadius; dx++) {
    for (int dz = -loadRadius; dz <= loadRadius; dz++) {
      int worldX = (playerChunkX + dx) * s;
      int worldZ = (playerChunkZ + dz) * s;

      if (getChunkFromChunkPos(worldX, worldZ) == nullptr) {
        chunks.push_back(std::make_unique<Chunk>(perlinNoise, this, worldX, worldZ));
        addPosition(worldX, worldZ);
        addNeighbours(worldX, worldZ);
      }
    }
  }

  std::sort(chunksToRebuild.begin(), chunksToRebuild.end());
  chunksToRebuild.erase(
    std::unique(chunksToRebuild.begin(), chunksToRebuild.end()),
    chunksToRebuild.end()
  );

  for (const auto& [worldX, worldZ] : chunksToRebuild) {
    if (Chunk* chunk = getChunkFromChunkPos(worldX, worldZ)) {
      chunk->rebuildMesh();
    }
  }
}
