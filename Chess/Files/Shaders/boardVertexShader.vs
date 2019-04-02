#version 440 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform float scale;

out VS_OUT
{
   vec2 texCoords;
}vs_out;

uniform vec2 offset;

void main()
{
   gl_Position = vec4((aPos * scale) + offset, 0.0, 1.0);
   vs_out.texCoords = aTexCoords;
}