#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "stb_image.h"
#include "world.hpp"
#include "perlinNoise.hpp"
#include "renderer.hpp"
#include "camera.hpp"



const int scr_width = 1280;
const int scr_height = 720;

float currentFrame = 0.0f, lastFrame = 0.0f, deltaTime = 0.0f;

bool firstMouse = true;
float lastX, lastY;

World* world_p;

glm::vec3 movementDirection(0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset = lastY - (float)ypos;

  lastX = (float)xpos;
  lastY = (float)ypos;

    Camera* camera = static_cast<Camera*>(
        glfwGetWindowUserPointer(window)
    );

    camera->processMouse(
        static_cast<float>(xoffset),
        static_cast<float>(yoffset)
    );
}

float fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  fov -= (float)yoffset;
  if(fov < 10.0f) fov = 10.0f;
  if(fov > 90.0f) fov = 90.0f;

   Camera* camera = static_cast<Camera*>(
        glfwGetWindowUserPointer(window)
    );

  camera->setFov(fov);
}

void frameBuffSize_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
  
  Camera* camera = static_cast<Camera*>(
    glfwGetWindowUserPointer(window)
  );

  camera->setAspectRatio((float)width/(float)height);
}

void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action != GLFW_PRESS && action != GLFW_REPEAT) return;

  if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window,true);
  if(key == GLFW_KEY_F2) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(key == GLFW_KEY_F3) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods ){
  Camera* camera = static_cast<Camera*>(
    glfwGetWindowUserPointer(window)
  );
  if(action != GLFW_PRESS ) return;
  glm::vec3 rayDir = glm::normalize(camera->getFront());
  RaycastResult raycast = world_p->raycast(camera->getPosition(), rayDir, 6.0f);
  if(raycast.hit){
    if(button == GLFW_MOUSE_BUTTON_RIGHT) world_p->placeBlock(raycast.px, raycast.py, raycast.pz, STONE);
    else if(button == GLFW_MOUSE_BUTTON_LEFT) world_p->breakBlock(raycast.bx, raycast.by, raycast.bz);
  }
}

int main(){
  if(!glfwInit()){
    std::cerr << "Error glfwInit()";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow( scr_width, scr_height, "Minecraft", NULL, NULL);

  if(!window){
    std::cerr << "Error glfwCreateWindow()";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, input_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback(window, frameBuffSize_callback);
  glfwSetMouseButtonCallback(window, mouseButton_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cerr << "Error load glad";
    glfwTerminate();
    return -1;
  }

  PerlinNoise noise;
  World world(&noise);
  world_p = &world;

  Player player(
    glm::vec3(0.0f, 30.0f, 0.0f),
    glm::vec3(1.0f, 2.0f, 1.0f),
    &world
  );

  Camera camera(
    player.getPosition() + glm::vec3(0.0f, 1.5f, 0.0f),
    45.0f,
    (float)scr_width/(float)scr_height,
    0.1f,
    1000.0f
  );

  glfwSetWindowUserPointer(window, &camera);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);
  int nbChannels, width, height;
  unsigned char* data = stbi_load("assets/atlas.png", &width, &height, &nbChannels, 0);

  if(!data){
    std::cerr << "Error loading texture \n";
    glfwTerminate();
    return -1;
  }else {
    int frmt = nbChannels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, frmt, width, height, 0,frmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(data);
  glActiveTexture(GL_TEXTURE0);

  glm::vec3 lightPos = glm::vec3(8.0f, 20.0f, 8.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 0.95f, 0.85f);

  Renderer renderer;

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.4f, 0.6f, 0.9f, 1.0f);

  while(!glfwWindowShouldClose(window)){
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::vec3 forward = camera.getFront();
    glm::vec3 right   = camera.getRight();
    glm::vec3 up      = camera.getUp();

    movementDirection = glm::vec3(0.0f);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      movementDirection += forward;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      movementDirection -= forward;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      movementDirection += right;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      movementDirection -= right;


    if(glm::length(movementDirection) > 0.0f)
      movementDirection = glm::normalize(movementDirection);

    static bool spaceWasPressed = false;

    bool spaceIsPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if(spaceIsPressed && !spaceWasPressed) player.jump();
    spaceWasPressed = spaceIsPressed;

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    renderer.render(world, player, view, projection);

    player.update(movementDirection, deltaTime);

    world.update(player.getPosition());

    camera.updatePosition(player);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
