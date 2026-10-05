#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
out vec3 fPos;
out vec2 fUV;

uniform vec2 offset;
uniform float time;

void main()
{
   fPos = aPos;
   fUV = aUV;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);
}