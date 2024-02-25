#include "config.h"  // 중요 헤더 파일
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
#include "Icon.h"
#include "Background.h"
#include "White.h"
#include "texture.h"
#include "Turntable.h"
#include "Speaker.h"
#include "Guitar.h"
#include "Arm.h"
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
	// 아래로 갈 수록 위에 그려짐
	// 배경
	initTransform();
	background.setTransform();
	transmit();
	background.bindVertex();
	background.modelOutput();

	// 스피커
	if (playFunc) {
		if (funcNumber == 0 || funcNumber == 1) {
			initTransform();
			speaker.setTransform();
			transmit();
			speaker.bindVertex();
			speaker.modelOutput();
		}
	}

	// Alcy
	for (int i = 0; i < ALCY_PART; i++) {
		initTransform();
		alcy.setTransform(i);
		transmit();
		alcy.bindVertex(i);
		alcy.modelOutput(i);
	}

	// 턴테이블
	if (playFunc) {
		if (funcNumber == 0 || funcNumber == 1) {
			initTransform();
			turntable.setTransform();
			transmit();
			turntable.bindVertex();
			turntable.modelOutput();
		}
		// 기타, 팔
		else if (funcNumber == 2) {
			initTransform();
			guitar.setTransform();
			transmit();
			guitar.bindVertex();
			guitar.modelOutput();

			initTransform();
			arm.setTransform();
			transmit();
			arm.bindVertex();
			arm.modelOutput();
		}
	}

	// ZZZ 이미지
	for (int i = 0; i < 3; i++) {
		if (zzz[i].zzzTransparent > 0.0) {
			initTransform();
			zzz[i].setTransform();
			transmit();
			zzz[i].bindVertex();
			zzz[i].modelOutput();
		}
	}

	// UI
	for (int i = 0; i < UI_PART; i++) {
		initTransform();
		ui.setTransform(i);
		transmit();
		ui.bindVertex(i);
		ui.modelOutput(i);
	}

	// 메뉴 아이콘
	for (int i = 0; i < ICON_PART; i++) {
		if (icon[i].iconTransparent > 0.0) {
			initTransform();
			icon[i].setTransform(i);
			transmit();
			icon[i].bindVertex();
			icon[i].modelOutput();
		}
	}
	
	//흰 배경
	if (whiteTransparent > 0.0) {
		initTransform();
		white.setTransform();
		transmit();
		white.bindVertex();
		white.modelOutput();
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
		//glDepthFunc(GL_ALWAYS);
		stbi_set_flip_vertically_on_load(true);

		makeShaderProgram();

		result = FMOD::System_Create(&ssystem);
		if (result != FMOD_OK)
			exit(0);

		if (INTRO == 0)  // 개발 시에는 인트로 생략
			gameStarted = true;
		else
			gameStarted = false;

		initFmod();
	}

	background.setBuffer();  // 배경 초기화
	for (int i = 0; i < ALCY_PART; i++) // 알키 버퍼 초기화
		setBufferAlcy(i);  
	for (int i = 0; i < UI_PART; i++) // UI 버퍼 초기화
		setBufferUI(i); 
	for(int i = 0; i < 3; i ++)  // zzz오브젝트 초기화
		zzz[i].setBuffer();
	for (int i = 0; i < ICON_PART; i++)  // 메뉴 아이콘 초기화
		icon[i].setBuffer();
	white.setBuffer();  // 흰 배경 초기화
	turntable.setBuffer();  // 턴테이블 초기화
	speaker.setBuffer(); // 스피커 초기화
	guitar.setBuffer(); // 기타 초기화
	arm.setBuffer(); // 팔 초기화

	// 텍스처 설정
	background.setTexture();  // 배경
	setAlcyTexture();  // 알키
	setUITexture();  // UI

	for (int i = 0; i < 3; i++) {  // zzz오브젝트
		zzz[i].setTexture();
		zzz[i].setDelay(10 * i);
	}

	for (int i = 0; i < ICON_PART; i++)  // menu icon
		icon[i].setTexture(i);
	
	white.setTexture();  // white
	turntable.setTexture();  // turntable
	speaker.setTexture();  // speaker
	guitar.setTexture();
	arm.setTexture();

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
