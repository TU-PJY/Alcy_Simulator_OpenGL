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

GLuint VAO_ALCY[ALCY_PART], VAO_UI[UI_PART], VBO;  // MODEL_COUNT는 config.h에 정의되어있음
BITMAPINFO* bmp;

// ui 리소스
unsigned int back, cursor[3];
unsigned int icon[3];
unsigned int tip;

// 알키 리소스
unsigned int alcyTail, alcyBody, alcyHair, alcyHead[3];
unsigned int eye[5], dot[3], eyeClose[3], brow[3], blink[3];

// 프롭 리소스
unsigned int zzz;

unsigned char* texture_data;

GLfloat plate[][48] = {  // 이미지 출력에 사용할 plate
	-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
	0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
	0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	-0.8f, 0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,
	-0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0
};

void vertexInput(int idx) {  // vertex
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(plate), plate, GL_STATIC_DRAW);
}

void setBufferUI(int idx) {
	glGenVertexArrays(1, &VAO_UI[idx]);
	glBindVertexArray(VAO_UI[idx]);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vertexInput(idx);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
	glEnableVertexAttribArray(2);
}

void setBufferAlcy(int idx) {  // 알키 버퍼 초기화
	glGenVertexArrays(1, &VAO_ALCY[idx]);
	glBindVertexArray(VAO_ALCY[idx]);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	vertexInput(idx);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
	glEnableVertexAttribArray(2);
}

void parameteri() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setAlcyTexture() {
	// tail
	glGenTextures(1, &alcyTail);
	glBindTexture(GL_TEXTURE_2D, alcyTail);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//tail.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// alcy body
	glGenTextures(1, &alcyBody);
	glBindTexture(GL_TEXTURE_2D, alcyBody);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//body.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// alcy hair
	glGenTextures(1, &alcyHair);
	glBindTexture(GL_TEXTURE_2D, alcyHair);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//hair.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// alcy head middle
	glGenTextures(1, &alcyHead[0]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//head_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// alcy head left
	glGenTextures(1, &alcyHead[1]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//head_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// alcy head right
	glGenTextures(1, &alcyHead[2]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//head_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);



	// eye middle
	glGenTextures(1, &eye[0]);
	glBindTexture(GL_TEXTURE_2D, eye[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//eye_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// eye left
	glGenTextures(1, &eye[1]);
	glBindTexture(GL_TEXTURE_2D, eye[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//eye_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// eye right
	glGenTextures(1, &eye[2]);
	glBindTexture(GL_TEXTURE_2D, eye[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//eye_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// eye squeak
	glGenTextures(1, &eye[3]);
	glBindTexture(GL_TEXTURE_2D, eye[3]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//eye_squeak.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// eye tired
	glGenTextures(1, &eye[4]);
	glBindTexture(GL_TEXTURE_2D, eye[4]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//eye_tired.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// dot middle
	glGenTextures(1, &dot[0]);
	glBindTexture(GL_TEXTURE_2D, dot[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//dot_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// dot left
	glGenTextures(1, &dot[1]);
	glBindTexture(GL_TEXTURE_2D, dot[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//dot_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// dot right
	glGenTextures(1, &dot[2]);
	glBindTexture(GL_TEXTURE_2D, dot[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//dot_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);



	//brow middle
	glGenTextures(1, &brow[0]);
	glBindTexture(GL_TEXTURE_2D, brow[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//brow_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//brow left
	glGenTextures(1, &brow[1]);
	glBindTexture(GL_TEXTURE_2D, brow[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//brow_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//brow right
	glGenTextures(1, &brow[2]);
	glBindTexture(GL_TEXTURE_2D, brow[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//brow_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//blink middle
	glGenTextures(1, &blink[0]);
	glBindTexture(GL_TEXTURE_2D, blink[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//blink_middle.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//blink left
	glGenTextures(1, &blink[1]);
	glBindTexture(GL_TEXTURE_2D, blink[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//blink_left.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//blink right
	glGenTextures(1, &blink[2]);
	glBindTexture(GL_TEXTURE_2D, blink[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//alcy//face//blink_right.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1500, 1500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);
}

void setUITexture() {
	// cursor
	glGenTextures(1, &cursor[0]);
	glBindTexture(GL_TEXTURE_2D, cursor[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//cursor.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 100, 100, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// cursor hand
	glGenTextures(1, &cursor[1]);
	glBindTexture(GL_TEXTURE_2D, cursor[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//cursor_hand.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 100, 100, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// cursor finger
	glGenTextures(1, &cursor[2]);
	glBindTexture(GL_TEXTURE_2D, cursor[2]);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//cursor_finger.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// exit icon
	glGenTextures(1, &icon[0]);
	glBindTexture(GL_TEXTURE_2D, icon[0]);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//icon_exit.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// info icon
	glGenTextures(1, &icon[1]);
	glBindTexture(GL_TEXTURE_2D, icon[1]);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//icon_info.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// tip
	glGenTextures(1, &tip);
	glBindTexture(GL_TEXTURE_2D, tip);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//tip.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 500, 500, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	// background
	glGenTextures(1, &back);  
	glBindTexture(GL_TEXTURE_2D, back);
	parameteri();
	texture_data = LoadDIBitmap("res//ui//background.bmp", &bmp);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 2560, 1440, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);
}