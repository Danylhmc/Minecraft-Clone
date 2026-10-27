#version 330 core 

layout(location = 0) in vec2 apos;

uniform mat4 model;

void main(){
  gl_Position = model * vec4(apos, 0.0, 1.0);
}
