#version 440 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out VS_OUT
{
   vec2 texCoords;
}vs_out;

uniform mat4 model;

void main()
{
   gl_Position = model * vec4(aPos, 0.0, 1.0);
   vs_out.texCoords = aTexCoords;
}