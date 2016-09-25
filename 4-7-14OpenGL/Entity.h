#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "ShaderHelpers.h"
#include "Shape.h"


class Entity
{
public:
	int prevTime;
	glm::vec3 curPos;
	glm::vec3 scale;
	glm::vec3 rot;
	float rotF;
	float rotS;
	glm::vec3 velocity;
	glm::vec3 force;
	Shape* shape;
	glm::mat4 worldMatrix;
	bool isSpring;
	glm::vec3 mousePos;

	//methods
	Entity(Shape *shape);
	void Draw(void);
	void Update(void);
	Entity(void);
	~Entity(void);
};

