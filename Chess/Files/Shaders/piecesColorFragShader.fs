#version 440 core

out vec4 fragColor;

uniform sampler2D image;
uniform vec3 color;

in VS_OUT
{
   vec2 texCoords;
}fs_in;

void main()
{
   vec4 result = texture(image, fs_in.texCoords);
   if(result.a < 0.5)
      discard;
   result.rgb = color;   
   fragColor = result;
}

