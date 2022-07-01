#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogldev_math_3d.h"


// // Original test (configuring visual studio)
//int main(int argc, char* argv[]) {
//	printf("Hello OpenGL");
//	glutInit(&argc, argv);
//	glutCreateWindow("GLEW Test");
//	GLenum err = glewInit();
//	if (GLEW_OK != err)
//	{
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
//	getchar();
//	return 0;
//}

// // Original RenderSceneCB first tutorial
//static void RenderSceneCB() {
//	static GLclampf c = 0.0f;
//	glClearColor(c, c, c, c);
//	printf("%f\n", c);
//	c += 1.0f / 256.0f;
//
//	if (c >= 1.0f) {
//		c = 0.0f;
//	}
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	glutPostRedisplay(); // allows from fram changes
//	glutSwapBuffers();
//}

GLuint VBO;

// Code for tutorial 2
//static void RenderSceneCB() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glDrawArrays(GL_POINTS, 0, 1);
//
//	glDisableVertexAttribArray(0);
//
//	glutSwapBuffers();
//}
//
//static void CreateVertexBuffer() {
//	Vector3f Vertices[1];
//	Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//}

static void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void CreateVertexBuffer() {
	glEnable(GL_CULL_FACE); // just having this makes the triangle disappear, makes the triangles be able to disappear
	glFrontFace(GL_CW); // makes the front face the one that appears, on Clock wise vertices GL_CCW would make counter clockwise vertices appear
	glCullFace(GL_FRONT); // makes only the front face, defined in last line, appear
	Vector3f Vertices[3];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f); // bottom left
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f); // top
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f); // bottom right
	//			  Vector3f(x, y, z);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;

	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: %s'\n", ShaderType, InfoLog);
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

const char* pVSFileName = "../shader.vs";
const char* pFSFileName = "../shader.fs";

static void CompileShaders() {
	GLuint ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	std::string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	}
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	int width = 500;
	int height = 400;
	glutInitWindowSize(width, height);

	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);
	int win = glutCreateWindow("Tutorial 03");
	printf("Window id : %d\n", win);

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);

	CreateVertexBuffer();

	CompileShaders();

	glutDisplayFunc(RenderSceneCB);


	glutMainLoop();
	return 0;
}