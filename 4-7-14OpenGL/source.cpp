#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "ShaderHelpers.h"
#include "Shape.h"
#include "Entity.h"
#include <time.h>      

using namespace std;

//global variables
GLuint indexVBO;
GLuint indexVAO;
const GLsizei numVert = 8;
const GLsizei numDataVal = numVert * 2;
GLuint program;
Shape * hexagon;
Shape * triangle;
Shape * rectangle;
Shape * circle;
glm::vec3 mousePos;

//entities for the simulation
Entity * enemy;
Entity * bucket;
Entity * bucketStripe;
Entity * ball1;
Entity * ball2;
Entity * ball3;
Entity * tally;

int score = 0;

void rotateBucket(unsigned char key, int x, int y)
{
	if(key == ' ')
	{
		bucket->rot.x = 0;
		bucket->rot.y = 1;
		bucket->rot.z = 0;
		bucket->rotS = .3f;

		bucketStripe->rot.x = 0;
		bucketStripe->rot.y = 1;
		bucketStripe->rot.z = 0;
		bucketStripe->rotS = .1f;
	}
	else
	{
		bucket->rotS = 0.0f;
		bucketStripe->rotS = 0.0f;
	}
}


void mouseMove(int x, int y)
{
	mousePos.x = ((float)x)/800;
	mousePos.x = (mousePos.x * 2) - 1;

	mousePos.y = ((float)y)/600;
	mousePos.y = ((mousePos.y * 2) - 1) * -1;
}

bool collisionDetect(glm::vec3 pos1, glm::vec3 pos2)
{
	//simple collision detection, not true detection though
	if(abs(pos1.x - pos2.x) < .2 && abs(pos1.y - pos2.y) < .2)
	{
		return true;
	}
	
	return false;
}

void init()
{
	//reading shaders in
	program = loadShaderProgram("Shaders/vertexShader.glsl","Shaders/fragmentShader.glsl");
	if(program)
	{
		glUseProgram(program);
		setShaderColor(program, "color", 0.0f, 0.0f, 1.0f); //make it blue
	}

	//create some shapes
	//Hexagon
	GLfloat vertexArray[] = {
		-0.5f, 1.0f,
		 0.5f,1.0f,
		 1.0f,0.5f,
		 1.0f,-0.5f,
		 0.5f,-1.0f,
		-0.5f,-1.0f,
		 -1.0f,-0.5f,
		 -1.0f,0.5f,
	};
	hexagon = new Shape(vertexArray, numVert * 2, program);

	//Triangle
	GLfloat vertexArray2[] = {
		 -0.5f,  0.5f, // Vertex 1 (X, Y)
		 0.5f, 0.0f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
	};
	triangle = new Shape(vertexArray2,6,program);

	//rectangle
	GLfloat vertexArray3[] = {
		 0.5f,  0.5f, // Vertex 1 (X, Y)
		 0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f,  // Vertex 3 (X, Y)
		-0.5f, 0.5f  // Vertex 4 (X, Y)
	};
	rectangle = new Shape(vertexArray3,8,program);


	//make some entities
	ball1 = new Entity(hexagon);
	ball2 = new Entity(hexagon);
	ball3 = new Entity(hexagon);
	bucket = new Entity(rectangle);
	bucketStripe = new Entity(rectangle);
	tally = new Entity(rectangle);
	enemy = new Entity(triangle);

	//set up some physics for the entities
	//bucket - follows mouse and springs around
	bucket->isSpring = true;
	bucket->scale.x = .3f;
	bucket->scale.y = .5f;
	bucket->scale.z = .5f;

	bucketStripe->scale.x = .3f;
	bucketStripe->scale.y = .1f;
	bucketStripe->scale.z = .5f;

	//balls
	//normal ball
	ball1->velocity.y = -.001f;
	ball1->force.y = -.001f;
	ball1->scale.x = .08f;
	ball1->scale.y = .1f;
	ball1->scale.z = .1f;
	ball1->rotS = .1f;

	//faster ball
	ball2->velocity.y = -.01f;
	ball2->force.y = -.001f;
	ball2->scale.x = .08f;
	ball2->scale.y = .1f;
	ball2->scale.z = .1f;
	ball2->rotS = .1f;

	//spinning balls
	ball3->velocity.y = -.01f;
	ball3->force.y = -.001f;
	ball3->scale.x = .08f;
	ball3->scale.y = .1f;
	ball3->scale.z = .1f;
	ball3->rot.x = 1;
	ball3->rot.y = 1;
	ball3->rot.z = 1;
	ball3->rotS = .1f;

	//tally mark
	tally->scale.x = .01f;
	tally->scale.y = .15f;
	tally->scale.z = .1f;

	//enemies
	enemy->scale.x = .3f;
	enemy->scale.y = .3f;
	enemy->scale.z = .3f;
	enemy->curPos.y = 0;
	enemy->curPos.x = -1;
	enemy->velocity.x = .1f;
	enemy->rotS = .5f;
	enemy->rot.z = 0;
	enemy->rot.x = 1;
	enemy->rot.y = 0;

	glClearColor(0.392f,0.584f,0.929f,1.0f);
}

void update()
{
	glutPostRedisplay();

	//update entities
	tally->Update();

	bucket->mousePos = mousePos;
	bucket->Update();
	bucketStripe->curPos = bucket->curPos;
	bucketStripe->Update();

	//ball updates
	ball1->Update();
	if(ball1->curPos.y < -1)
	{
		ball1->curPos.y = 1;
		ball1->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}

	ball2->Update();
	if(ball2->curPos.y < -1)
	{
		ball2->curPos.y = 1;
		ball2->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}

	ball3->Update();
	if(ball3->curPos.y < -1)
	{
		ball3->curPos.y = 1;
		ball3->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}
	//limit ball speed
	if(ball1->velocity.y < -.075f) { ball1->velocity.y = -.075f; }
	if(ball2->velocity.y < -.15f) { ball2->velocity.y = -.15f; }
	if(ball3->velocity.y < -.15f) { ball3->velocity.y = -.15f; }

	//enemy track bucket
	if(enemy->curPos.y < bucket->curPos.y)
	{
		enemy->force.y = .05f;
	}
	if(enemy->curPos.y > bucket->curPos.y)
	{
		enemy->force.y = -.05f;
	}
	enemy->Update();
	if(enemy->curPos.x > 1)
	{
		enemy->curPos.x = -1;
		enemy->curPos.y = 0;
	}
	if(enemy->curPos.y > 1 || enemy->curPos.y < -1)
	{
		enemy->velocity.y = 0;
	}

	//collision detection
	if(collisionDetect(bucket->curPos, ball1->curPos))
	{
		score++;
		ball1->curPos.y = 1;
		ball1->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}
	if(collisionDetect(bucket->curPos, ball2->curPos))
	{
		score++;
		ball2->curPos.y = 1;
		ball2->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}
	if(collisionDetect(bucket->curPos, ball3->curPos))
	{
		score++;
		ball3->curPos.y = 1;
		ball3->curPos.x = -1 + ((float)rand() / float(RAND_MAX/2));
	}
	if(collisionDetect(bucket->curPos, enemy->curPos))
	{
		score = 0;
		enemy->curPos.x = -1;
		enemy->curPos.y = 0;
	}
}

void draw()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	//draw using entities
	setShaderColor(program, "color", 1.0f, 0.0f, 0.0f); //make it red
	tally->curPos.x = -.9f;
	tally->curPos.y = .9f;
	tally->Update();
	for(int i = 0; i < score; i++)
	{
		tally->curPos.x += .02f;
		tally->Update();
		tally->Draw();
	}

	enemy->Draw();

	setShaderColor(program, "color", 1.0f, 1.0f, 0.0f); //make it yellow
	ball1->Draw();
	setShaderColor(program, "color", 0.0f, 1.0f, 0.0f); //make it green
	ball2->Draw();
	setShaderColor(program, "color", 1.0f, 1.0f, 1.0f); //make it white
	ball3->Draw();

	setShaderColor(program, "color", 0.5f, 0.35f, 0.05f); //make it brown
	bucket->Draw();
	setShaderColor(program, "color", 0.0f, 0.0f, 0.0f); //make it brown
	bucketStripe->curPos.y += .1f;
	bucketStripe->Update();
	bucketStripe->Draw();
	bucketStripe->curPos.y -= .2f;
	bucketStripe->Update();
	bucketStripe->Draw();

	setShaderColor(program, "color", 0.0f, 0.0f, 1.0f); //make it blue
	glFlush();
}

int main(int argc, char** argv)
{
	//set up random seed
	srand (time(NULL));

	glewExperimental = GL_TRUE;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutInitContextVersion(4,3);
	//if 4.3 doesnt work at home, try 4.2, 4.1, or 4.0
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Awesome title!");

	if(glewInit() != GLEW_OK ) { return -1; }

	//mouse move
	glutPassiveMotionFunc(mouseMove);

	//will call update when the main loop is idle
	glutIdleFunc(update);
	glutKeyboardFunc(rotateBucket);

	//will call draw() when redisplaying the window
	glutDisplayFunc(draw);

	//call init after OpenGL has been set up but before the main loop starts
	init();

	//start GLUT's main loop which will call update and draw
	glutMainLoop();

	return 0;
}