// 모델 버퍼 
#include "buffer.h"
#include "texture.h"

GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info) {
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;

	//--- 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	//--- 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}

	//--- 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}

	//--- BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	//--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}

	//--- 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}

	//--- 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * (*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);

	//--- 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}

	//--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}

	fclose(fp);
	return bits;
}


GLuint VAO[IMAGE_COUNT], VBO;  // MODEL_COUNT는 config.h에 정의되어있음
BITMAPINFO* bmp;
unsigned int back, cursor;
unsigned int alcyTail, alcyBody, alcyHair, alcyHead[3];
unsigned char* texture_data;

int sx, sy;  // 이미지 사이즈

GLfloat plate[][48] = {  // 이미지 출력에 사용할 plate
	-0.8f, -0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
	0.8f, -0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
	0.8f, 0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	0.8f, 0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	-0.8f, 0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,
	-0.8f, -0.8f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0, 0.0
};

void vertexInput(int idx) {  // vertex
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(plate), plate, GL_STATIC_DRAW);
}

void setBuffer(int idx) {
	glGenVertexArrays(1, &VAO[idx]);
	glBindVertexArray(VAO[idx]);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vertexInput(idx);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); //텍스처 좌표 속성 
	glEnableVertexAttribArray(2);
}

void parameteri() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setTexture() {
	glGenTextures(1, &cursor);  // cursor
	glBindTexture(GL_TEXTURE_2D, cursor);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//cursor.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 100, 100, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &back);  // background
	glBindTexture(GL_TEXTURE_2D, back);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//background.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2560, 1440, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyTail);  // tail
	glBindTexture(GL_TEXTURE_2D, alcyTail);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//alcy//tail.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyBody);  // alcy body
	glBindTexture(GL_TEXTURE_2D, alcyBody);
	parameteri(); 
	texture_data = LoadDIBitmap("res//alcy//alcy//body.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHair);  // alcy hair
	glBindTexture(GL_TEXTURE_2D, alcyHair);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//alcy//hair.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[0]);  // alcy head middle
	glBindTexture(GL_TEXTURE_2D, alcyHead[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//alcy//head_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[1]);  // alcy head left
	glBindTexture(GL_TEXTURE_2D, alcyHead[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//alcy//head_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[2]);  // alcy head right
	glBindTexture(GL_TEXTURE_2D, alcyHead[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//alcy//head_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);
}