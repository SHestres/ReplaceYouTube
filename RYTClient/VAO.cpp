#include "VAO.h"


VAO::VAO()
{
	//Create the VAO, allowing ID to be assigned
	glGenVertexArrays(1, &ID);
}

//Configures a certain attribute within the VAO
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	//Bind vbo so it can be used
	VBO.Bind();

	//Configure VAO: First position, num values per vertex, type of values, only matters if int type, distance between vertices, offset (where the vertices start in the array)
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
	delete(this);
}