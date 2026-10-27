#pragma once 

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class Shader{
public:
  Shader(const char* Vertex, const char* Fragment);
  ~Shader();
  void checkForCompilationErrors(unsigned int shader, const char* name);
  void setBool(const char* name, int value) const;
  void setInt(const char* name, int value) const;
  void setFloat(const char* name, float value) const;
  void setVec3(const char* name, glm::vec3 &vec) const;
  void setVec4(const char* name, glm::vec4 &vec) const;
  void setMat3(const char* name, glm::mat3 &mat) const;
  void setMat4(const char* name, glm::mat4 &mat) const;  
  
  void use() const;
private:
  unsigned int ID;
};
