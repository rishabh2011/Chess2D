#version 440 core

layout (location = 0) in vec2 aPos;

uniform float scale;

void main()
{
    gl_Position = vec4(aPos * scale, 0.0, 1.0);
}

