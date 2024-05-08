// 버퍼 초기화
#pragma once
#include "HEADER.h"

extern GLuint VBO;
extern unsigned char* texture_data;


void set_canvas(GLuint &VAO);

void set_texture(unsigned int &tex, const char* directory, int width, int height, int channel);

void set_text(unsigned int& tex, std::string type);
void draw_image(unsigned int tex, GLuint VAO);

GLvoid build_font(const char* fontName, int fontSize, int type, GLuint& base, HDC& hDC);
GLvoid kill_text(GLuint base);
int set_font(const char* font_name, int size, int type, GLuint& base, HDC& hDC);
GLvoid draw_text(unsigned int tex, GLuint base, const char* fmt, ...);
bool install_font(const std::string& fontFilePath);