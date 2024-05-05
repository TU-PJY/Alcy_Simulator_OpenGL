// 셰이더 프로그램 생성
#include "../header/shader.h"


GLchar* vertex_source, * fragment_source;
GLuint vertex_shader, fragment_shader;
GLuint ID;


char* load_buffer(const char* file) {
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


void set_vertex_shader() {
	vertex_source = load_buffer("GLSL//GLSL_vertex.glsl");
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
	glCompileShader(vertex_shader);
	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader err\n" << errorLog << std::endl;
		return;
	}
}


void set_fragment() {
	fragment_source = load_buffer("GLSL//GLSL_fragment.glsl");
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);
	glCompileShader(fragment_shader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader err\n" << errorLog << std::endl;
		return;
	}
}


void set_shader() {
	set_vertex_shader();
	set_fragment();
	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(ID);
}
