#version 330 core 

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 textCoords;


uniform sampler2D texture0;

uniform vec3 lightColor;
uniform vec3 lightDir;

void main(){
  vec3 norm = normalize(Normal);
  vec3 L = normalize(-lightDir);

  vec3 ambiant = lightColor * 0.5;

  float diff = max(dot(norm, L), 0.0);
  vec3 diffuse = lightColor * diff * 0.5;

  vec3 textColor = texture(texture0, textCoords).rgb;
  FragColor = vec4((diffuse + ambiant) * textColor, 1.0);
}
