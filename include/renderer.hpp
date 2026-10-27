#pragma once 

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include "world.hpp"
#include "player.hpp"
#include "shader.hpp"
#include "camera.hpp"

class World;
class Player;
class Shader;

class Renderer{
public:
  Renderer();

  void render(
    const World& world,
    const Player& player,
    glm::mat4& view,
    glm::mat4& projection
  );

  void drawWorld(const World& world);
  void drawPlayer(const Player& player, glm::mat4& view, glm::mat4& projection);
  void CreateCrosshairMesh();
  void drawCrosshair();
private:
  void createPlayerMesh();
  Shader worldShader;
  Shader playerShader;
  Shader crosshairShader;

  unsigned int playerVAO;
  unsigned int playerVBO;

  unsigned int crosshairVao;
  unsigned int crosshairVbo;

  glm::vec3 lightColor = glm::vec3(1.0f, 0.97f, 0.9f);
  glm::vec3 lightDir = glm::normalize(glm::vec3(-0.4f, -1.0f, -0.6f));
};
