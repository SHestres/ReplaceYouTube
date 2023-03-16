#include "VBO.h"


VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	//Create the buffer
	glGenBuffers(1, &ID);
	//Bind it so data can be assigned
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//Assign the data
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
	delete(this);
}