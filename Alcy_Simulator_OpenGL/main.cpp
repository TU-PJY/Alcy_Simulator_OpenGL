﻿#include "config.h"  // 중요 헤더 파일
#include "shader.h"  // 셰이더 생성
#include "buffer.h"  // 버퍼
#include "transform.h"  // 변환
#include "gl_func.h"  // GL 기능 함수
#include "screen.h"  // 윈도우 사이즈
#include "sound.h"  // 사운드
#include "globalVar.h"
#include "Alcy.h"  
#include "UI.h"
#include "ZZZ.h"
#include "texture.h"
#include "stb_image.h"

int WIDTH = GetSystemMetrics(SM_CXSCREEN);
int HEIGHT = GetSystemMetrics(SM_CYSCREEN);  // 화면 사이즈에 맞추어 창을 출력한다

GLvoid displayReshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid displayOutput() {
	glClearColor(0.39, 0.40, 0.50, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ID);

	setWindowView();

	// Alcy
	for (int i = 0; i < ALCY_PART; i++) {
		initTransform();
		alcy.setTransform(i);
		transmit();
		alcy.bindVertex(i);
		alcy.modelOutput(i);
	}

	// ZZZ 이미지
	for (int i = 0; i < 3; i++) {
		initTransform();
		zzz[i].setTransform();
		transmit();
		zzz[i].bindVertex();
		zzz[i].modelOutput();
	}

	// UI
	for (int i = 0; i < UI_PART; i++) {
		initTransform();
		ui.setTransform(i);
		transmit();
		ui.bindVertex(i);
		ui.modelOutput(i);
	}

	glutSwapBuffers();
}

void main(int argc, char** argv) {
	{  // fold here
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GL_MULTISAMPLE);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Totally Smoll Alcy Simulator");
		glutFullScreen();  // 전체화면으로 전환한다
		glutSetCursor(GLUT_CURSOR_NONE);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cerr << "Unable to initialize GLEW" << endl;  exit(EXIT_FAILURE);
		}
		else cout << "GLEW Initialized" << endl;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		stbi_set_flip_vertically_on_load(true);

		makeShaderProgram();

		result = FMOD::System_Create(&ssystem);
		if (result != FMOD_OK)
			exit(0);

		initFmod();

		if (INTRO == 0)  // 개발 시에는 인트로 생략
			gameStarted = true;
		else
			gameStarted = false;
	}


	for (int i = 0; i < UI_PART; i++) // UI 버퍼 초기화
		setBufferUI(i); 
	for (int i = 0; i < ALCY_PART; i++)
		setBufferAlcy(i);  // 알키 버퍼 초기화
	for(int i = 0; i < 3; i ++)
		zzz[i].setBuffer();

	setAlcyTexture();  // 텍스처 설정
	setUITexture();
	for (int i = 0; i < 3; i++) {
		zzz[i].setTexture();
		zzz[i].setDelay(10 * i);
	}

	stbi_image_free(texture_data);
	
	glutDisplayFunc(displayOutput);
	glutReshapeFunc(displayReshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(pMotion);
	glutMouseWheelFunc(Wheel);

	glutTimerFunc(10, timerOperation, 1);
	glutMainLoop();
}
