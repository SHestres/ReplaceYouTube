#version 330 core

// Sets the framework for the two different pieces of info that are gleaned from the VAO
// The "hook" for VAO.LinkAttrib()
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// A value that can be input directly
uniform float scale;

void main()
{
   gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
}