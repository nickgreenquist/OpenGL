#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

char * loadTextFile(const char* file);
GLuint loadShader(const char* file, GLenum shaderType);
GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);
void setShaderVec2(GLuint shaderProgram, const char* name, glm::vec2 vecToSend);
void setShaderColor(GLuint shaderIndex, const char* varName, float red, float green, float blue);
void setShaderMatrix(GLuint shaderProgram, const char* name, glm::mat4 vecToSend);