#include "../header/image_text_util.h"
#include "../header/view.h"
#include "../header/shader.h"

GLuint VBO;
unsigned char* texture_data;  // 텍스처 저장에 사용되는 임시 변수

GLfloat vertex[][48] = {  // 이미지 출력에 사용할 vertex
	-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0,
	0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.0,
	0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
	-0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,
	-0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0
};


void set_vertex(GLuint &VAO) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // 텍스처 좌표 속성 
	glEnableVertexAttribArray(2);
}


void set_texture(unsigned int& tex, const char* directory, int width, int height, int channel) {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture_data = stbi_load(directory, &width, &height, &channel, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	stbi_image_free(texture_data);
}


void transmit_translation() {
	result = r_mat * t_mat * s_mat;  // 최종 변환

	transperancy_location = glGetUniformLocation(ID, "transparency");
	glUniform1f(transperancy_location, alpha);

	projection_location = glGetUniformLocation(ID, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);

	view_location = glGetUniformLocation(ID, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);

	obj_color_location = glGetUniformLocation(ID, "objectColor");
	glUniform4f(obj_color_location, 0.0, 0.0, 0.0, 1.0);

	viewpos_location = glGetUniformLocation(ID, "viewPos"); // viewPos 값 전달: 카메라 위치
	glUniform3f(viewpos_location, camera_pos.x, camera_pos.y, camera_pos.z);

	model_location = glGetUniformLocation(ID, "model"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(model_location, 1, GL_FALSE, value_ptr(result)); // 변환 값 적용하기
}


void draw_image(unsigned int tex) {
	transmit_translation();

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


bool install_font(const std::string& fontFilePath) {
	// Add the font resource
	int result = AddFontResource(fontFilePath.c_str());
	if (result == 0) {
		std::cerr << "Failed to install font." << std::endl;
		return false;
	}

	// Send WM_FONTCHANGE message to notify other applications of the font change
	HWND hwnd = nullptr;  // Broadcast to all top-level windows
	SendMessage(hwnd, WM_FONTCHANGE, 0, 0);

	return true;
}