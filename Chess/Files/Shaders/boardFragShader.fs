#version 440 core

out vec4 fragColor;

in VS_OUT
{
   vec2 texCoords;
}fs_in;

uniform sampler2D image;

void main()
{
   vec4 result = texture(image, fs_in.texCoords);
   if(result.a < 0.5)
      discard;
   result.rgb = pow(result.rgb, vec3(1.0 / 2.2));
   fragColor = result;
}

