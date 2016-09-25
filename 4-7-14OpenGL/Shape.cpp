#include "Shape.h"
#include "ShaderHelpers.h"


//shape constructor does much of the work of setting up array buffers to draw polygons
Shape::Shape(GLfloat vertices[], int numVertices, GLuint programIndex)
{
	numV = numVertices;
	progIndex = programIndex;

	//bind the VAO
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	//bind vertex buffer
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	float a = 0.0f;
	glBufferData(GL_ARRAY_BUFFER,sizeof(a) * numV,vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);

}

Shape::Shape(void)
{
	//error
}

//destructor deletes the vao and vbo
Shape::~Shape(void)
{
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
}


//draw method access the shader to positon and scale polygons
void Shape::Draw(glm::mat4 worldMatrix)
{
	setShaderMatrix(progIndex,"worldMatrix",worldMatrix);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN,0,numV);
}
