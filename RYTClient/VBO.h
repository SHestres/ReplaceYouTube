#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO 
{
	public:
		GLuint ID;
		//Creates VBO and assigns it data
		VBO(GLfloat* vertices, GLsizeiptr size);

		//Bind VBO to make it active (so data can be assigned)
		void Bind();
		void Unbind();
		void Delete();
};

#endif VBO_CLASS_H