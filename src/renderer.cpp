#include "renderer.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
  :worldShader(Shader("shaders/vertex.glsl","shaders/fragment.glsl")),
  playerShader(Shader("shaders/vertexPlayer.glsl", "shaders/fragmentPlayer.glsl")),
  crosshairShader(Shader("shaders/crosshairVertex.glsl", "shaders/crosshairFragment.glsl")){
  createPlayerMesh();
  CreateCrosshairMesh();
}

void Renderer::render(
  const World& world,
  const Player& player,
  glm::mat4& view,
  glm::mat4& projection
){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  worldShader.use();
  worldShader.setMat4("view", view);
  worldShader.setMat4("projection", projection);
  worldShader.setVec3("lightColor", lightColor);
  worldShader.setVec3("lightDir", lightDir);

  drawWorld(world);
  drawCrosshair();
  //drawPlayer(player, view, projection);
}

void Renderer::drawWorld(const World& world){
  world.draw();
}

void Renderer::drawPlayer(const Player& player, glm::mat4& view, glm::mat4& projection){
  glm::vec3 position = player.getPosition();
  glm::vec3 size = player.getSize() * 0.5f;

  glm::mat4 model(1.0f);
  model = glm::translate(
    model,
    position + glm::vec3(0.0f, size.y, 0.0f)
  );
  model = glm::scale(model, size);

  playerShader.use();
  playerShader.setMat4("model", model);
  playerShader.setMat4("view", view);
  playerShader.setMat4("projection", projection);

  glBindVertexArray(playerVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::createPlayerMesh(){
  glGenVertexArrays(1, &playerVAO);
  glGenBuffers(1, &playerVBO);

  glBindVertexArray(playerVAO);
  glBindBuffer(GL_ARRAY_BUFFER, playerVBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);
}

void Renderer::CreateCrosshairMesh(){
  float crosshairVertices[] = {
		-0.01f,  0.0f,
		0.01f,  0.0f,
		0.0f, -0.02f,
		0.0f,  0.02f,
	};

  glGenBuffers(1, &crosshairVbo);
  glGenVertexArrays(1, &crosshairVao);

  glBindVertexArray(crosshairVao);
  glBindBuffer(GL_ARRAY_BUFFER, crosshairVbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2 , GL_FLOAT, false, 2 * sizeof(float), 0);
  
  glEnableVertexAttribArray(0);
}

void Renderer::drawCrosshair(){

  glm::mat4 crosshairModel = glm::mat4(5.0f);

  glDisable(GL_DEPTH_TEST);
		  crosshairShader.use();
      crosshairShader.setMat4("model", crosshairModel);
		  glBindVertexArray(crosshairVao);
      glLineWidth(2.0f);
		  glDrawArrays(GL_LINES, 0, 4);
		  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH_TEST);
}

