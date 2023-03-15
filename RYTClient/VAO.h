#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"


class VAO
{
	public:
		GLuint ID;
		VAO();

		//Configures a certain attribute within the VAO. (How the shader reads the VBO, linking a layout to a part of the array)
		void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

		//Bind the VAO to make it currently active
		void Bind();

		//Unbind the VAO
		void Unbind();
		void Delete();
};

#endif VAO_CLASS_H
