#include "shader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* Vertex, const char* Fragment){
  std::fstream vFile, fFile;
  vFile.open(Vertex);
  if(!vFile){
    std::cerr << "Error : failed to find path to the vertex file \n";
    return;
  }
  fFile.open(Fragment);
  if(!fFile){
    std::cerr << "Error : failed to find path to the fragment file \n";
    return;
  }

  std::stringstream vStream, fStream;
  vStream << vFile.rdbuf();
  fStream << fFile.rdbuf();

  std::string vString, fString;
  vString = vStream.str();
  fString = fStream.str();

  const char* vCode = vString.c_str();
  const char* fCode = fString.c_str();

  unsigned int VERTEX, FRAGMENT;
  VERTEX = glCreateShader(GL_VERTEX_SHADER);
  FRAGMENT = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(VERTEX, 1, &vCode, NULL);
  glShaderSource(FRAGMENT, 1, &fCode, NULL);

  glCompileShader(VERTEX);
  checkForCompilationErrors(VERTEX, "VERTEX");
  glCompileShader(FRAGMENT);
  checkForCompilationErrors(FRAGMENT, "FRAGMENT");

  ID = glCreateProgram();
  glAttachShader(ID, VERTEX);
  glAttachShader(ID, FRAGMENT);

  glLinkProgram(ID);

  int success;
  char infoLog[1024];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(ID, 1024, NULL, infoLog);
    std::cerr << "Error : LINKING PROGRAM :" << infoLog << "\n";
  }
}

Shader::~Shader(){
  glDeleteProgram(ID);
}

void Shader::use() const
{
  glUseProgram(ID);
}

void Shader::setBool(const char* name, int value)const
{
  glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setInt(const char* name, int value)const
{
  glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value)const
{
  glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVec3(const char* name, glm::vec3 &vec) const
{
  glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const char* name, glm::vec4 &vec) const
{
  glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void Shader::setMat3(const char* name, glm::mat3 &mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(mat));
}

void Shader::setMat4(const char* name, glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1,false,  glm::value_ptr(mat));
}


void Shader::checkForCompilationErrors(unsigned int shader, const char* type)
{
  int success;
  char infoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    std::cerr << "Error : COMPILE SHADER OF TYPE '" << type << "':" << infoLog << "\n";
  }
}
