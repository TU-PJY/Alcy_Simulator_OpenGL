#include "config.h"  // 중요 헤더 파일
#include "shader.h"  // 셰이더 생성
#include "buffer.h"  // 버퍼
#include "translate.h"  // 변환
#include "gl_func.h"  // GL 기능 함수
#include "screen.h"  // 윈도우 사이즈

extern GLuint ID;

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

	// IMAGE_COUNT는 config.h에 정의되어있음
	for (int i = 0; i < IMAGE_COUNT; i++) {  // IMAGE_COUNT 만큼 반복문을 돌며 변환과 출력 반복
		setTransform(i);  // 변환 세팅
		finishTransform(i); // 변환을 glsl로 전달
		modelOutput(i);  // 최종 출력, 3개 함수 모두 imageOutput.cpp에 있음
	}
	
	glutSwapBuffers();
}

void main(int argc, char** argv) {
	{  // fold here
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GL_MULTISAMPLE);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Totally Cute Alcy Simulator");
		glutFullScreen();  // 전체화면으로 전환한다
		glutSetCursor(GLUT_CURSOR_NONE);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cerr << "Unable to initialize GLEW" << endl;  exit(EXIT_FAILURE);
		}
		else cout << "GLEW Initialized" << endl;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		makeShaderProgram();
	}

	// MODEL_COUNT는 config.h에 정의되어있음
	for (int i = 0; i < IMAGE_COUNT; i++) // IMAGE_COUNT만큼 버퍼 초기화
		setBuffer(i);  // imageBuffer.cpp에 있음
	setTexture();  // 텍스처 설정
	
	glutDisplayFunc(displayOutput);
	glutReshapeFunc(displayReshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(pMotion);

	glutTimerFunc(10, timerOperation, 1);
	glutMainLoop();
}
