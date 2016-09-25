#include "Entity.h"


Entity::Entity(void)
{

}

Entity::Entity(Shape* s)
{
	shape = s;

	//set up glm vars
	scale.x = 1; scale.y = 1; scale.z = 1;
	rot.x = 0; rot.y = 1; rot.z = 0;
	force.x = 0; force.y = 0; force.z = 0;
	rotF = 0.0f; rotS = 0.0f;
	curPos.x = 0; curPos.y = 0; curPos.z = 0;
	velocity.x = 0; velocity.y = 0; velocity.z = 0;
	prevTime = 0;

	isSpring = false;
}


Entity::~Entity(void)
{

}

void Entity::Update(void)
{
	//for animations
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = currentTime - prevTime;
	float dtScaled = dt * .01f;
	float k = 5;
	float damp = .98f;
	float mass = 2;

	glm::vec3 disp = curPos - mousePos;
	glm::vec3 springForce = (-1 * k) * disp;
	springForce  = springForce - (damp * velocity);
	springForce = springForce / mass;

	if(isSpring) { force = springForce; }

	velocity = velocity + (force * dtScaled);
	curPos  = curPos + (velocity * dtScaled);

	prevTime = currentTime;

	rotF += rotS;

	worldMatrix = glm::translate(curPos) * glm::rotate(rotF,rot) * glm::scale(scale);
}

void Entity::Draw(void)
{
	shape->Draw(worldMatrix);
}
