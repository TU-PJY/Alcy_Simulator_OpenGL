// 모델 버퍼 
#define STB_IMAGE_IMPLEMENTATION
#include "buffer.h"
#include "texture.h"
#include "stb_image.h"

GLuint VAO_ALCY[ALCY_PART], VAO_UI[UI_PART], VBO;  // MODEL_COUNT는 config.h에 정의되어있음

// ui 리소스
unsigned int cursor[3];
unsigned int uiIcon[3];
unsigned int bar;
unsigned int tip;
unsigned int title;

// 알키 리소스
unsigned int alcyTail, alcyBody, alcyHair, alcyHead[9];
unsigned int eye[5], dot[3], eyeClose[3], brow[3], blink[3];

unsigned char* texture_data;

//ui
int channel = 1;
int titleW = 1500, titleH = 1500;
int cursorW = 200, cursorH = 200;
int iconW = 512, iconH = 512;
int tipW = 500, tipH = 500;
int barW = 512, barH = 512;

// 알키
int alcyW = 1500, alcyH = 1500;


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
	stbi_set_flip_vertically_on_load(true);

	// tail
	glGenTextures(1, &alcyTail);
	glBindTexture(GL_TEXTURE_2D, alcyTail);
	parameteri();
	texture_data = stbi_load("res//alcy//tail.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// alcy body
	glGenTextures(1, &alcyBody);
	glBindTexture(GL_TEXTURE_2D, alcyBody);
	parameteri();
	texture_data = stbi_load("res//alcy//body.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// alcy hair
	glGenTextures(1, &alcyHair);
	glBindTexture(GL_TEXTURE_2D, alcyHair);
	parameteri();
	texture_data = stbi_load("res//alcy//hair.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// alcy head middle
	glGenTextures(1, &alcyHead[0]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[0]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_middle.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// alcy head left
	glGenTextures(1, &alcyHead[1]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[1]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_left.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// alcy head right
	glGenTextures(1, &alcyHead[2]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[2]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_right.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//alcy head house
	glGenTextures(1, &alcyHead[3]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[3]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_house.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//alcy head glitch //////////////////
	glGenTextures(1, &alcyHead[4]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[4]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_glitch_1.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[5]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[5]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_glitch_2.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[6]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[6]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_glitch_3.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[7]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[7]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_glitch_4.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	glGenTextures(1, &alcyHead[8]);
	glBindTexture(GL_TEXTURE_2D, alcyHead[8]);
	parameteri();
	texture_data = stbi_load("res//alcy//head_glitch_5.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	///////////////


	// eye middle
	glGenTextures(1, &eye[0]);
	glBindTexture(GL_TEXTURE_2D, eye[0]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//eye_middle.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// eye left
	glGenTextures(1, &eye[1]);
	glBindTexture(GL_TEXTURE_2D, eye[1]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//eye_left.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// eye right
	glGenTextures(1, &eye[2]);
	glBindTexture(GL_TEXTURE_2D, eye[2]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//eye_right.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// eye squeak
	glGenTextures(1, &eye[3]);
	glBindTexture(GL_TEXTURE_2D, eye[3]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//eye_squeak.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// eye tired
	glGenTextures(1, &eye[4]);
	glBindTexture(GL_TEXTURE_2D, eye[4]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//eye_tired.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// dot middle
	glGenTextures(1, &dot[0]);
	glBindTexture(GL_TEXTURE_2D, dot[0]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//dot_middle.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// dot left
	glGenTextures(1, &dot[1]);
	glBindTexture(GL_TEXTURE_2D, dot[1]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//dot_left.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// dot right
	glGenTextures(1, &dot[2]);
	glBindTexture(GL_TEXTURE_2D, dot[2]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//dot_right.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);



	//brow middle
	glGenTextures(1, &brow[0]);
	glBindTexture(GL_TEXTURE_2D, brow[0]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//brow_middle.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//brow left
	glGenTextures(1, &brow[1]);
	glBindTexture(GL_TEXTURE_2D, brow[1]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//brow_left.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//brow right
	glGenTextures(1, &brow[2]);
	glBindTexture(GL_TEXTURE_2D, brow[2]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//brow_right.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//blink middle
	glGenTextures(1, &blink[0]);
	glBindTexture(GL_TEXTURE_2D, blink[0]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//blink_middle.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//blink left
	glGenTextures(1, &blink[1]);
	glBindTexture(GL_TEXTURE_2D, blink[1]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//blink_left.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	//blink right
	glGenTextures(1, &blink[2]);
	glBindTexture(GL_TEXTURE_2D, blink[2]);
	parameteri();
	texture_data = stbi_load("res//alcy//face//blink_right.png", &alcyW, &alcyH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1500, 1500, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
}

void setUITexture() {
	// title
	glGenTextures(1, &title);
	glBindTexture(GL_TEXTURE_2D, title);
	parameteri();
	texture_data = stbi_load("res//ui//title.png", &titleW, &titleH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, titleW, titleH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// cursor
	glGenTextures(1, &cursor[0]);
	glBindTexture(GL_TEXTURE_2D, cursor[0]);
	parameteri();
	texture_data = stbi_load("res//ui//cursor.png", &cursorW, &cursorH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// cursor hand
	glGenTextures(1, &cursor[1]);
	glBindTexture(GL_TEXTURE_2D, cursor[1]);
	parameteri();
	texture_data = stbi_load("res//ui//cursor_hand.png", &cursorW, &cursorH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// cursor finger
	glGenTextures(1, &cursor[2]);
	glBindTexture(GL_TEXTURE_2D, cursor[2]);
	parameteri();
	texture_data = stbi_load("res//ui//cursor_finger.png", &cursorW, &cursorH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cursorW, cursorH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// exit icon
	glGenTextures(1, &uiIcon[0]);
	glBindTexture(GL_TEXTURE_2D, uiIcon[0]);
	parameteri();
	texture_data = stbi_load("res//ui//icon_exit.png", &iconW, &iconH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iconW, iconH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// info icon
	glGenTextures(1, &uiIcon[1]);
	glBindTexture(GL_TEXTURE_2D, uiIcon[1]);
	parameteri();
	texture_data = stbi_load("res//ui//icon_info.png", &iconW, &iconH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iconW, iconH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// tip
	glGenTextures(1, &tip);
	glBindTexture(GL_TEXTURE_2D, tip);
	parameteri();
	texture_data = stbi_load("res//ui//tip.png", &tipW, &tipH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tipW, tipH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

	// bar
	glGenTextures(1, &bar);
	glBindTexture(GL_TEXTURE_2D, bar);
	parameteri();
	texture_data = stbi_load("res//ui//bar.png", &barW, &barH, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, barW, barH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
}