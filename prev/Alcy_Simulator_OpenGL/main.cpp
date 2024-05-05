#include "config.h"  // 중요 헤더 파일
#include "shader.h"  // 셰이더 생성
#include "buffer.h"  // 버퍼
#include "transform.h"  // 변환
#include "texture.h"  // 텍스처
#include "gl_func.h"  // GL 기능 함수
#include "screen.h"  // 윈도우 사이즈
#include "sound.h"  // 사운드
#include "globalVar.h"  // 프로젝트 전역 변수
#include "Start.h"  // 게임 시작 작업
#include "main2.h"  // 오브젝트 세팅 및 출력

int WIDTH = GetSystemMetrics(SM_CXSCREEN);  // 화면 사이즈에 맞추어 창을 출력한다
int HEIGHT = GetSystemMetrics(SM_CYSCREEN);  

GLvoid displayReshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid displayOutput() {
	glClearColor(0.39, 0.40, 0.50, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ID);

	setWindowView();

	if(LoadingEnd)
		objectOutput();  // 로딩이 모두 끝난 순간부터 인게임 오브젝트 출력을 시작한다.

	if (INTRO == 1) {
		if (!gameStarted)  // 게임 시작 전까지는 로고 및 로딩 화면을 출력한다. 게임 시작 시 더 이상 출력하지 않는다.
			for (int i = 0; i < 3; i++)
				start.objectOut(i);  // 로딩 화면 출력
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
		stbi_set_flip_vertically_on_load(true);  // 이미지 뒤집힘 방지

		makeShaderProgram();

		result = FMOD::System_Create(&ssystem);
		if (result != FMOD_OK)
			exit(0);

		if (INTRO == 0) {  // 개발 모드, 로딩화면 및 인트로를 건너뜀
			gameStarted = true;
			LoadingEnd = true;
			initObject();
			ssystem->init(64, FMOD_INIT_NORMAL, extradriverdata);
			initFmod();
			glutKeyboardFunc(keyDown);  // glut 기능 활성화
			glutKeyboardUpFunc(keyUp);
			glutMouseFunc(Mouse);
			glutMotionFunc(Motion);
			glutPassiveMotionFunc(pMotion);
			glutMouseWheelFunc(Wheel);

			stbi_image_free(texture_data);

			ssystem->playSound(mainTheme, 0, false, &channelTheme);
		}
		else {  // 배포 모드, 로딩화면 및 인트로를 거침
			gameStarted = false;
			ssystem->init(64, FMOD_INIT_NORMAL, extradriverdata);
			ssystem->createSound("res//sound//UI//logo.wav", FMOD_DEFAULT, 0, &logoSound); // 로고 사운드를 출력해야 하므로 먼저 로딩한다
			start.setBuffer();
			start.setTexture();
		}
	}
	
	glutDisplayFunc(displayOutput);
	glutReshapeFunc(displayReshape);
	glutTimerFunc(10, timerOperation, 1);

	glutMainLoop();

}
