#include "chunk.hpp"
#include "block.h"
#include "world.hpp"

Chunk::Chunk(PerlinNoise* perlinNoise, World* world, int chunkX, int chunkZ)
  :world(world){
  pos.x = chunkX;
  pos.z = chunkZ;

  auto fmb = [&](double x, double z) -> double {
    double amp = 20.0;
    double freq = 0.7;
    double value = 0.0;
    double maxValue = 0.0;

    int octaves = 3;
    double lacunarity = 2.0;
    double persistence = 0.4;

    for(int i = 0; i < octaves ; i++){
      value += perlinNoise->noise( x * freq * 0.05, z * freq * 0.05) * amp;
      maxValue += amp;
      freq *= lacunarity;
      amp *= persistence;
    }
    return value / maxValue;
  };

  for(int x = 0; x < chunk_size; x++){
    for(int z = 0; z < chunk_size; z++){
      int wx = pos.x + x;
      int wz = pos.z + z;
      int surfaceY = 8 + static_cast<int>(fmb(wx, wz) * 20.0f);
      for(int y = 0; y < chunk_height; y++){
        if(y < surfaceY - 4) blocks[x][y][z] = STONE;
        else if(y < surfaceY) blocks[x][y][z] = DIRT;
        else if(y == surfaceY) blocks[x][y][z] = GRASS;
        else blocks[x][y][z] = AIR;
      }
    }
  }
}

Chunk::~Chunk(){
  if(VAO){
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }

  if(VBO){
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }
}

void Chunk::rebuildMesh(){
  if(VAO){
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }

  if(VBO){
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  setMesh();
}

bool Chunk::isTransparent(int x, int y, int z){
  if(x >= 0 && x < chunk_size &&
     y >= 0 && y < chunk_height &&
     z >= 0 && z < chunk_size){
    return blocks[x][y][z] == AIR;
  }

  int worldX = pos.x + x;
  int worldZ = pos.z + z;
  return world->getBlock(worldX, y, worldZ) == AIR;
}

BlockType Chunk::getBlock(int x, int y, int z){
  if(x < 0 || x >= chunk_size || y < 0 || y >= chunk_height || z < 0 || z >= chunk_size)
    return AIR;
  return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, BlockType type){
  if(x < 0 || x >= chunk_size || y < 0 || y >= chunk_height || z < 0 || z >= chunk_size)
    return;
  blocks[x][y][z] = type;
}

void Chunk::setMesh(){
  std::vector<float> vertices;

  for(int x = 0; x < chunk_size; x++){
    for(int z = 0; z < chunk_size; z++){
      for(int y = 0; y < chunk_height; y++){
        if(blocks[x][y][z] == AIR) continue;

        int worldX = pos.x + x;
        int worldZ = pos.z + z;
        if(isTransparent( x + 1, y, z)) addFace(&vertices, worldX, y, worldZ, RIGHT, blocks[x][y][z]);
        if(isTransparent( x - 1, y, z)) addFace(&vertices, worldX, y, worldZ, LEFT, blocks[x][y][z]);
        if(isTransparent( x, y + 1, z)) addFace(&vertices, worldX, y, worldZ, TOP, blocks[x][y][z]);
        if(isTransparent( x, y - 1, z)) addFace(&vertices, worldX, y, worldZ, BOTTOM, blocks[x][y][z]);
        if(isTransparent( x, y, z + 1)) addFace(&vertices, worldX, y, worldZ, BACK, blocks[x][y][z]);
        if(isTransparent( x, y, z - 1)) addFace(&vertices, worldX, y, worldZ, FRONT, blocks[x][y][z]);
      }
    }
  }

  vertexCount = vertices.size() / 8;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

void Chunk::addFace(std::vector<float>* vertices, int x, int y, int z, FaceSide side, int type){
  int rowU;
  float offsetU = 0.0f;
  for(int i = 0; i < 6 ; i ++){
    int offset = i * 8;
    float blockScale = 0.5f;
    vertices->push_back(cubeFaces[side][offset + 0] * blockScale + x);
    vertices->push_back(cubeFaces[side][offset + 1] * blockScale + y);
    vertices->push_back(cubeFaces[side][offset + 2] * blockScale + z);

    vertices->push_back(cubeFaces[side][offset + 3]);
    vertices->push_back(cubeFaces[side][offset + 4]);
    vertices->push_back(cubeFaces[side][offset + 5]);

    rowU = cubeFaces[side][offset + 6];

    switch(type){
      case STONE:
        offsetU = 0.75f;
      break;
      case GRASS:
        if(side == RIGHT || LEFT || FRONT || BACK){
          offsetU = 0.25f;
        }
        if(side == TOP){
          offsetU = 0.5f;
        }
      break;
      default:
        offsetU = 0.0f;
    }

    vertices->push_back(rowU * 0.25 + offsetU);
    vertices->push_back(cubeFaces[side][offset + 7]);
  }
}

void Chunk::draw(){
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
