// 셰이더 프로그램 생성
#include "shader.h"

GLchar* vertexSource, * fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint ID;

char* loadBuffer(const char* file) {
	FILE* fptr; long length; char* buf;

	fptr = fopen(file, "rb");
	if (!fptr) return NULL;

	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void makeVertexShader() {
	vertexSource = loadBuffer("GLSL_vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader err\n" << errorLog << endl;
		return;
	}
}

void makeFragProgram() {
	fragmentSource = loadBuffer("GLSL_fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader err\n" << errorLog << endl;
		return;
	}
}

void makeShaderProgram() {
	makeVertexShader();
	makeFragProgram();
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(ID);
}
