#pragma once 

#include "player.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
  Camera(
    glm::vec3 position,
    float fov,
    float aspectRatio,
    float nearPlane,
    float farPlane
  );

  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  void setAspectRatio(float aspectRatio);

  void setFov(float fov);
  float getFov();

  glm::vec3 getPosition() const;

  glm::vec3 getFront() const;
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;

  void processMouse(float xoffset, float yoffset);
  void updateVectors();

  void updatePosition(Player& player);

private:
  glm::vec3 position;

  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;

  float yaw = 0.0f, pitch = 0.0f;

  float fov;
  float aspectRatio;

  float nearPlane;
  float farPlane;
};
