#version 330 core

layout(location = 0)in vec3 inPosition;
layout(location = 1)in vec3 colour;

out vec3 exColour;

void main()
{
  gl_Position = vec4(inPosition, 1.0);
  exColour = colour;
}
