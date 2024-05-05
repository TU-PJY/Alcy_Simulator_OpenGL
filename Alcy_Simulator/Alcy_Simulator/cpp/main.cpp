#define STB_IMAGE_IMPLEMENTATION
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

// display size
int WIDTH = GetSystemMetrics(SM_CXSCREEN);
int HEIGHT = GetSystemMetrics(SM_CYSCREEN);

GLvoid gl_main() {
	glClearColor(0.39f, 0.40f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glUseProgram(ID);
	
	cam.update_camera();
	set_view();

	fw.routine();

	glutPostRedisplay();
	glutSwapBuffers();
}


GLvoid display_reshape(int w, int h) {
	glViewport(0, 0, w, h);
}


void main(int argc, char** argv) {
	{  // fold here
		// 게임에 필요한 폰트 설치
		install_font("res//font//Somatic-Rounded.otf");
		install_font("res//font//joystix monospace.otf");

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GL_MULTISAMPLE);

		glutInitWindowPosition(0, 0);
		glutInitWindowSize(WIDTH, HEIGHT);

		glutCreateWindow("Totally Smoll Alcy Simulator V2");

		glutSetCursor(GLUT_CURSOR_NONE);
		// convert to fullscreen
		glutFullScreen();

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
		stbi_set_flip_vertically_on_load(true);

		set_shader();
	}
	
	glutDisplayFunc(gl_main);
	glutReshapeFunc(display_reshape);

	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);

	glutMouseFunc(mouse_button);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_passive_motion);
	glutMouseWheelFunc(mouse_wheel);

	fw.init(home_mode, "home_mode");
	glutMainLoop();
}
