#define STB_IMAGE_IMPLEMENTATION

#include "../header/data_util.h"
#include "../header/shader.h"
#include "../header/view.h"
#include "../header/gl_func.h"
#include "../header/Camera.h"
#include "../header/mode_header.h"
#include "../header/sound.h"
#include "../header/image_text_util.h"

// framework
FWL fw;

//camera
Camera cam;

// GPU vendor
std::string vendor;

// shared vertex
GLuint VAO;

// display size
int WIDTH = 1600;
int HEIGHT = 900;

int FWIDTH = GetSystemMetrics(SM_CXSCREEN);
int FHEIGHT = GetSystemMetrics(SM_CYSCREEN);

clock_t start_time, end_time;

GLvoid display_reshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

GLvoid gl_main() {
	start_time = clock();

	glClearColor(0.39f, 0.40f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glUseProgram(ID);
	
	cam.update_camera();
	set_view();

	fw.Routine();

	glutPostRedisplay();
	glutSwapBuffers();


	end_time = clock();
	fw.InputFrameTime(double(end_time - start_time) / 1000);
}


void main(int argc, char** argv) {
	{  // fold here
		// 게임에 필요한 폰트 설치
		install_font("res//font//joystix monospace.otf");
		install_font("res//font//maniac.regular.ttf");

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

		glutInitWindowPosition(FWIDTH / 2 - WIDTH / 2, FHEIGHT / 2 - HEIGHT / 2);
		glutInitWindowSize(WIDTH, HEIGHT);

		glutCreateWindow("Totally Smoll Alcy Simulator V2");

		glutSetCursor(GLUT_CURSOR_NONE);

		// convert to fullscreen
		//glutFullScreen();

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cerr << "Unable to initialize GLEW" << std::endl;  
			exit(EXIT_FAILURE);
		}

		else std::cout << "GLEW Initialized" << std::endl;


		// get vendor info from GPU
		const GLubyte* vendor_info = glGetString(GL_VENDOR);
		if (vendor_info) {
			vendor = reinterpret_cast<const char*>(vendor_info);
			//std::cout << "vendor: " << vendor << std::endl;
		}


		glEnable(GL_MULTISAMPLE);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 이미지 로드 방향 반전 옵션
		stbi_set_flip_vertically_on_load(true);

		// 공유 버텍스 설정
		set_vertex(VAO);

		// 세이브 파일이 존재하지 않으면 세이브 파일을 신규 생성한다
		check_data_invalid();

		// 세이브 파일의 버전이 현재 버전보다 낮을 경우 현재 버전에 맞는 세이브 파일로 업데이트 한다
		check_data_version();

		// 사운드 파일 로드
		load_sound_file();

		// 셰이더 세팅
		set_shader();

		// 프레임워크 세팅
		fw.InitMainMode(game_mode, "game_mode");
	}
	
	glutDisplayFunc(gl_main);
	glutReshapeFunc(display_reshape);
	glutMainLoop();
}
