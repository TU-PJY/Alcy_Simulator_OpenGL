// 모델 버퍼 
#define STB_IMAGE_IMPLEMENTATION
#include "buffer.h"
#include "texture.h"

GLuint VBO;
unsigned char* texture_data;

GLfloat plate[][48] = {  // 이미지 출력에 사용할 plate
	-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
	0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
	0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	-0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,
	-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0
};

void vertexInput() {  // vertex
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(plate), plate, GL_STATIC_DRAW);
}

void parameteri() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



