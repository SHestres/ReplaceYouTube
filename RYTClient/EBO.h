#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	GLuint ID;
	//Creates the EBO, assigns a valid ID, and assigns its data
	EBO(GLuint* indices, GLsizeiptr size);

	//Bind to make active
	void Bind();
	void Unbind();
	void Delete();
};

#endif VBO_CLASS_H#pragma once
