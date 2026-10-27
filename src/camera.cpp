#include "camera.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <iostream>

Camera::Camera(
  glm::vec3 position,
    float fov,
    float aspectRatio,
    float nearPlane,
    float farPlane
):position(position),
  fov(fov),
  aspectRatio(aspectRatio),
  nearPlane(nearPlane),
  farPlane(farPlane)
{
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  right = glm::normalize(glm::cross(front, up));
}

glm::mat4 Camera::getViewMatrix(){
  return glm::lookAt(position, position + front, up);
};

glm::mat4 Camera::getProjectionMatrix(){
  return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setAspectRatio(float aspectRatio){
  this->aspectRatio = aspectRatio;
}

void Camera::setFov(float fov){
  this->fov = fov;
}

float Camera::getFov(){
  return fov;
}

glm::vec3 Camera::getPosition() const{
  return position;
}

glm::vec3 Camera::getFront() const{
  return front;
}

glm::vec3 Camera::getUp() const{
  return up;
}

glm::vec3 Camera::getRight() const{
return right;
}

void Camera::processMouse(float xoffset, float yoffset){
  float sensitivity = 0.08f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  pitch += yoffset;
  yaw += xoffset;

  if(pitch < -89.9f) pitch = -89.9f;
  if(pitch > 89.9f) pitch = 89.9f;

  updateVectors();
}

void Camera::updateVectors(){
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));

  front = glm::normalize(direction);
  right = glm::normalize(glm::cross(front, up));
}

void Camera::updatePosition(Player& player){
  position = player.getPosition() + glm::vec3(0.0f, 1.5f, 0.0f);
}
