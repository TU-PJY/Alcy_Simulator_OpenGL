// 타이머
#include "gl_func.h"

int lastElapsedTime, elapsedTime;
GLfloat fs;  // frame sync, 프레임 동기화

void syncFrame() {
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    fs = (elapsedTime - lastElapsedTime) / 10.0; // Convert milliseconds to seconds
    lastElapsedTime = elapsedTime;
}

void timerOperation(int value) {
    syncFrame();

    glutTimerFunc(10, timerOperation, 1);
    if (glutGetWindow() != 0)
        glutPostRedisplay();
}
