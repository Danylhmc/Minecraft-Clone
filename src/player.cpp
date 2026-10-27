#include "player.hpp"
#include "block.h"
#include <cmath>
#include <optional>
#include <utility>
#include <iostream>

Player::Player(glm::vec3 pos, glm::vec3 size, World* world)
  :position(pos), velocity(0.0f), size(size),onGround(false),world(world)
{
  std::cout << "size =" 
            << size.x << ","
            << size.y << ","
            << size.z << "\n";
}

Player::~Player(){}

glm::vec3 Player::getPosition() const
{
  return position;
}

glm::vec3 Player::getSize() const
{
  return size;
}

void Player::update(glm::vec3& movement, float dt){
  applyGravity(dt);

  if (movement.x != 0 || movement.z != 0) {
    velocity.x += movement.x * speed * dt;
    velocity.z += movement.z * speed * dt;

    float maxSpeed = 5.0f;
    float horizontalSpeed = (float)std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

    if (horizontalSpeed > maxSpeed) {
      float scale = maxSpeed / horizontalSpeed;
      velocity.x *= scale;
      velocity.z *= scale;
    }
  }else {
    velocity.x *= 0.8f;
    velocity.z *= 0.8f;

    if (std::abs(velocity.x) < 0.01f) velocity.x = 0;
    if (std::abs(velocity.z) < 0.01f) velocity.z = 0;
  }

  float dx = velocity.x * dt;
  float dz = velocity.z * dt;
  float dy = velocity.y * dt;

  moveX(dx);
  moveZ(dz);
  moveY(dy);
}

void Player::moveX(float dx){
  position.x += dx;

  std::optional<glm::vec3> blockPos = checkForCollisions(position, size);
  if(!blockPos.has_value()) return;

  if(dx > 0){
    position.x = blockPos->x - size.x;
    velocity.x = 0;
  }
  if(dx < 0){
    position.x = blockPos->x + 1;
    velocity.x = 0;
  }
}

void Player::moveZ(float dz){
  position.z += dz;

  std::optional<glm::vec3> blockPos = checkForCollisions(position, size);
  if(!blockPos.has_value()) return;

  if(dz > 0){
    position.z = blockPos->z - size.z;
    velocity.z = 0;
  }
  if(dz < 0){
    position.z = blockPos->z + 1;
    velocity.z = 0;
  }
}

void Player::moveY(float dy){
  position.y += dy;

  std::optional<glm::vec3> blockPos = checkForCollisions(position, size);
  if(!blockPos.has_value()) return;

  if(dy > 0){
    position.y = blockPos->y - size.y;
    velocity.y = 0;
  }
  if(dy < 0){
    position.y = blockPos->y + 1;
    velocity.y = 0;
    onGround = true;
  }
}

void Player::jump(){
  if(!onGround) return;
    velocity.y = 8.0f;
    onGround = false;
  }


void Player::applyGravity(float dt){
  velocity.y -= gravity * dt * 1.5f;
}

bool Player::intersects(glm::vec3 max, glm::vec3 min, int x, int y, int z){
  return(
    max.x > x && min.x < x + 1 &&
    max.z > z && min.z < z + 1 &&
    max.y > y && min.y < y + 1
  );
}

std::optional<glm::vec3> Player::checkForCollisions(glm::vec3 pos, glm::vec3 size){
  glm::vec3 min = pos;
  glm::vec3 max = pos + size;

  int maxX = (int)std::floor(max.x);
  int minX = (int)std::floor(min.x);

  int maxZ = (int)std::floor(max.z);
  int minZ = (int)std::floor(min.z);

  int maxY = (int)std::floor(max.y);
  int minY = (int)std::floor(min.y);

  for(int x = minX; x <= maxX; x++){
    for(int z = minZ; z <= maxZ; z++){
      for(int y = minY; y <= maxY; y++){
        if(intersects(max, min, x, y, z)){
          BlockType block = world->getBlock(x, y, z);
          if(block == AIR)continue;
          else{
            return glm::vec3(x, y, z);
          }
        }
      }
    }
  }
  return std::nullopt;
}


