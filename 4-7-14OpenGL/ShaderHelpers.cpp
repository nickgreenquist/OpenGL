#include <GL\glew.h>
#include <GL\freeglut.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using namespace std;

char * loadTextFile(const char* file)
{
	ifstream inFile(file,ios::binary);

	if(inFile.is_open() )
	{
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(ios::beg);

		char* fileContents = new char[length+1];
		fileContents[length] = 0;

		inFile.read(fileContents, length);

		fileContents[length] = 0;

		inFile.close();
		cout << fileContents;
		cout << "reached here\n";
		return fileContents;
	}
	return 0;
}

GLuint loadShader(const char* file, GLenum shaderType)
{
	const char* shader = loadTextFile(file);
	if(shader == 0)
	{
		return 0;
	}
	GLuint shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject,1,&shader,NULL);
	glCompileShader(shaderObject);
	GLint compiled = 0;
	glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&compiled);
	if(compiled)
	{
		return shaderObject;
	}
	else
	{
		GLint logLen;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLen);

		GLchar *compilerLog = new GLchar[logLen + 1];
		GLsizei slen = 0;



		glGetShaderInfoLog(shaderObject,logLen,0,compilerLog);
		cout << "compiler log:\n" << compilerLog;
		glDeleteShader(shaderObject);
		delete[] compilerLog;
		return 0;
	}
	return 1;
}

GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	GLuint vertexObject = loadShader(vertexFile,GL_VERTEX_SHADER);
	if(vertexObject == 0) { return 0; }
	GLuint fragementObject = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if(fragementObject == 0) { return 0; }

	GLuint ProgramObject = glCreateProgram();
	glAttachShader(ProgramObject, vertexObject);
	glAttachShader(ProgramObject, fragementObject);
	glLinkProgram(ProgramObject);

	GLint linked = 0;
	glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
	if(linked)
	{
		return ProgramObject;
	}
	else
	{
		GLchar *compilerLog = new GLchar[linked + 1];
		GLsizei slen = 0;

		glGetShaderInfoLog(ProgramObject,linked,&slen,compilerLog);
		cout << "compiler log:\n" << compilerLog;
		glDeleteProgram(ProgramObject);
		delete compilerLog;
		return 0;
	}
	return 1;
}

void setShaderColor(GLuint shaderIndex, const char* varName, float red, float green, float blue)
{
	GLuint varLoc =  glGetUniformLocation(shaderIndex, varName);
	glProgramUniform4f(shaderIndex, varLoc, red, green, blue, 1.0f);
}

void setShaderVec2(GLuint shaderProgram, const char* name, glm::vec2 vecToSend)
{
	GLuint varLoc =  glGetUniformLocation(shaderProgram, name);
	glProgramUniform2fv(shaderProgram,varLoc, 1,&vecToSend[0]);
}

void setShaderMatrix(GLuint shaderProgram, const char* name, glm::mat4 vecToSend)
{
	GLuint varLoc = glGetUniformLocation(shaderProgram,name);
	glProgramUniformMatrix4fv(shaderProgram,varLoc,1,GL_FALSE, &vecToSend[0][0]);
}