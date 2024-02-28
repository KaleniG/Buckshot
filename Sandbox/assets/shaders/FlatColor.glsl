#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;   

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main()
{
  gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f); 
}

#type fragment
#version 330 core

out vec4 a_Color;  

uniform vec4 u_Color;

void main()
{
  a_Color = u_Color;
}