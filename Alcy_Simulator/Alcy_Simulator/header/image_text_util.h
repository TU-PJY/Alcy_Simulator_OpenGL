// 버퍼 초기화
#pragma once
#include "HEADER.h"
#include "../header/view.h"
#include "../header/shader.h"

extern GLuint VBO, VAO;
extern unsigned char* texture_data;


void set_vertex(GLuint &VAO);
void set_texture(unsigned int &tex, const char* directory, int width, int height, int channel);
void draw_image(unsigned int tex);
void transmit_translation();
bool install_font(const std::string& fontFilePath);



enum text_mode {
	dynamic_out,
	static_out
};


class Text {
private:
	GLuint base{};
	HDC hDC{};

	const char* fmt{};

public:
	void out_dynamic(GLfloat r, GLfloat g, GLfloat b, const char* fmt, ...) {
		va_list args{};
		va_start(args, fmt);

		draw_text_dynamic(0.0, 0.0, 0.0, base, " ", args);
		draw_text_dynamic(r, g, b, base, fmt, args);

		va_end(args);
	}

	void out_static(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, const char* fmt, ...) {
		va_list args{};
		va_start(args, fmt);

		draw_text_static(0.0, 0.0, 0.0, 0.0, 0.0, base, " ", args);
		draw_text_static(x, y, r, g, b, base, fmt, args);

		va_end(args);
	}


	int set_font(const char* font_name, int size, int type, GLuint& base, HDC& hDC) {                     // All Setup For OpenGL Goes Here
		hDC = wglGetCurrentDC();            // 현재 openGL 윈도우의 hDC를 가져온다.
		build_font(font_name, size, type, base, hDC);

		return TRUE;                        // Initialization Went OK
	}


	GLvoid build_font(const char* fontName, int fontSize, int type, GLuint& base, HDC& hDC) {
		HFONT   font;     // Windows Font ID
		HFONT   oldfont;  // Used For Good House Keeping

		// calculate font size scale
		int dpiX = GetDeviceCaps(hDC, LOGPIXELSX);
		float scale = static_cast<float>(dpiX) / 96.0f;
		int result_size = static_cast<int>(fontSize * scale);

		base = glGenLists(96);  // Storage For 96 Characters

		font = CreateFont(-result_size, // Height Of Fonts
			0,              // Width Of Font
			0,              // Angle Of Escapement
			0,              // Orientation Angle
			type,        // Font Weight
			FALSE,          // Italic    
			FALSE,          // Underline
			FALSE,          // Strikeout
			ANSI_CHARSET,   // Character Set Identifier
			OUT_TT_PRECIS,  // Output Precision
			CLIP_DEFAULT_PRECIS,        // Clipping Precision
			ANTIALIASED_QUALITY,        // Output Quality
			FF_DONTCARE | VARIABLE_PITCH,  // Family And Pitch
			fontName);         // Font Name

		oldfont = (HFONT)SelectObject(hDC, font); // Selects The Font We Want
		wglUseFontBitmaps(hDC, 32, 96, base);     // Builds 96 Characters Starting At Character 32
		SelectObject(hDC, oldfont);               // Selects The Font We Want
		DeleteObject(font);                       // Delete The Font
	}


	GLvoid draw_text_dynamic(GLfloat r, GLfloat g, GLfloat b, GLuint base, const char* fmt, va_list args) {
		glBindTexture(GL_TEXTURE_2D, 0);

		if (vendor == "NVIDIA Corporation") {
			transmit_translation();
			obj_color_location = glGetUniformLocation(ID, "objectColor");
			glUniform3f(obj_color_location, r, g, b);

			glm::vec4 position = projection * view * result * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			if (position.x < -1.0f ||
				position.x > 1.0 ||
				position.y < -1.0 ||
				position.y > 1.0) {

				// 텍스트가 화면 모서리에 있는 경우 그리지 않음
				return;
			}
			glRasterPos2f(0.0, 0.0);
		}

		else {
			transmit_translation();

			glm::vec4 position = projection * view * result * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			glColor4f(r, g, b, alpha);
			glRasterPos2f(position.x, position.y);
		}


		if (fmt == NULL)                // If There's No Text
			return;                     // Do Nothing

		char text[256];          // Holds Our String
		vsprintf(text, fmt, args);    // And Converts Symbols To Actual Numbers

		glPushAttrib(GL_LIST_BIT);              // Pushes The Display List Bits
		glListBase(base - 32);                  // Sets The Base Character to 32

		glCallLists(GLsizei(strlen(text)), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
		glPopAttrib();                      // Pops The Display List Bits
	}


	GLvoid draw_text_static(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLuint base, const char* fmt, va_list args) {
		glBindTexture(GL_TEXTURE_2D, 0);

		if (vendor == "NVIDIA Corporation") {
			init_transform();
			set_object_static(dy(x), dy(y));

			transmit_translation();
			obj_color_location = glGetUniformLocation(ID, "objectColor");
			glUniform3f(obj_color_location, r, g, b);

			glRasterPos2f(0.0, 0.0);
		}
		
		else {
			transmit_translation();

			glColor4f(r, g, b, alpha);
			glRasterPos2f(x, y);
		}


		if (fmt == NULL)                // If There's No Text
			return;                     // Do Nothing

		char text[256];          // Holds Our String
		vsprintf(text, fmt, args);    // And Converts Symbols To Actual Numbers

		glPushAttrib(GL_LIST_BIT);              // Pushes The Display List Bits
		glListBase(base - 32);                  // Sets The Base Character to 32

		glCallLists(GLsizei(strlen(text)), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
		glPopAttrib();                      // Pops The Display List Bits
	}


	GLvoid kill_text(GLuint base) { glDeleteLists(base, 96); }


	Text(const char* fontName, int fontSize, int type) {
		set_font(fontName, fontSize, type, base, hDC);
	}


	~Text() {
		kill_text(base);
	}
};