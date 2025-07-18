#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	//Create the buffer
	glGenBuffers(1, &ID);
	//Bind it so data can be assigned
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//Assign the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
	delete(this);
}