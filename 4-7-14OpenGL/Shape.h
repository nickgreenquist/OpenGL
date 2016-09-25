#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using namespace std;
class Shape
{
	GLuint vbo,vao;
	int numV;
	GLuint progIndex;
	GLuint offset, scale;
public:
	Shape(GLfloat vertices[], int numVertices, GLuint programIndex);
	Shape(void);
	~Shape(void);
	void Draw(glm::mat4 worldMatrix);
};

