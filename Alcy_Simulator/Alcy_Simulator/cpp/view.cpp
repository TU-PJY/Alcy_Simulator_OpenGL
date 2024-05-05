#include "../header/view.h"
#include "../header/Camera.h"


// 변환 관련 변수들을 여기에 선언 및 사용
glm::vec3 camera_pos, camera_direction, camera_up;
glm::mat4 result, view, projection, s_mat, r_mat, t_mat;
GLfloat alpha;

GLfloat ratio;

unsigned int projection_location, view_location, model_location, viewpos_location, transperancy_location;


void set_view() {  // 시점 세팅using namespace glm;
    using namespace glm;

    view = mat4(1.0f);
    camera_pos = vec3(0.0f, 0.0f, 1.0f);
    camera_direction = vec3(0.0f, 0.0f, 0.0f);
    camera_up = vec3(0.0f, 1.0f, 0.0f);
    projection = mat4(1.0f);

    ratio = 1.0 * WIDTH / HEIGHT;  // 화면 비율을 구하여 모델이 제대로 나오도록 함
    // X축 변환에 곱해야함.

    view = lookAt(camera_pos, camera_direction, camera_up);

    view = translate(view, vec3(0.0, -1.0, 0.0));
    view = rotate(view, radians(cam.angle), vec3(0.0, 0.0, 1.0));
    view = translate(view, vec3(0.0, 1.0, 0.0));
    view = translate(view, vec3(cam.x, cam.y, 0.0));

    projection = ortho(-1.0 * ratio, 1.0 * ratio, -1.0, 1.0, -100.0, 100.0);
}


glm::mat4 move_image(GLfloat x, GLfloat y) {
    using namespace glm;
    mat4 matrix{ mat4(1.0f) };
    matrix = translate(matrix, vec3(x, y, 0.0));
    return matrix;
}


glm::mat4 rotate_image(GLfloat rad) {
    using namespace glm;
    mat4 matrix{ mat4(1.0f) };
    matrix = rotate(matrix, radians(rad), vec3(0.0, 0.0, 1.0));
    return matrix;
}


glm::mat4 scale_image(GLfloat x, GLfloat y) {
    using namespace glm;
    mat4 matrix{ mat4(1.0f) };
    matrix = scale(matrix, vec3(x, y, 0.0));
    return matrix;
}

void set_object_static(GLfloat x, GLfloat y) {
    t_mat *= move_image(-cam.x, -cam.y);
    t_mat *= move_image(0.0, -1.0);
    t_mat *= rotate_image(-cam.angle);
    t_mat *= move_image(0.0, 1.0);
    t_mat *= move_image(x, y);
}


void init_transform() {  // 변환 초기화
    using namespace glm;

    result = mat4(1.0f);  // 최종 행렬

    s_mat = mat4(1.0f);  // 신축 행렬
    r_mat = mat4(1.0f);  // 회전 행렬
    t_mat = mat4(1.0f);  // 이동 행렬

    alpha = 1.0f;
}