#pragma once 

#include <glm/glm.hpp>
#include <world.hpp>
#include <optional>

inline const float playerVertices[] = {
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,

  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
    // Right (+X)
  1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
    // Back (-Z)
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
    // Left (-X)
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,
    // Bottom (-Y)
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
    // Top (+Y)
  -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f
};
class Player{
public:
  World* world;
  glm::vec3 position;
  Player(glm::vec3 pos, glm::vec3 size, World* world);
  ~Player();

  glm::vec3 getPosition() const;
  glm::vec3 getSize() const;

  void update(glm::vec3& movement, float dt);

  void applyGravity(float dt);
  void moveX(float dx);
  void moveZ(float dz);
  void moveY(float dy);
  void jump();

  bool intersects(glm::vec3 max, glm::vec3 min, int x, int y, int z);
  std::optional<glm::vec3> checkForCollisions(glm::vec3 pos, glm::vec3 size);
private:
  glm::vec3 velocity;
  glm::vec3 size;
  float speed = 50.0f;
  float gravity = 9.81f;
  bool onGround = true;
};
