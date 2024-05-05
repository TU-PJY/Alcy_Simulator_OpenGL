﻿#pragma once
#include "HEADER.h"

extern glm::vec3 camera_pos, camera_direction, camera_up;
extern glm::mat4 result, view, projection, s_mat, r_mat, t_mat;
extern GLfloat alpha;

extern unsigned int projection_location, view_location, model_location, viewpos_location, transperancy_location;


void set_view();

glm::mat4 move_image(GLfloat x, GLfloat y);
glm::mat4 rotate_image(GLfloat rad);
glm::mat4 scale_image(GLfloat x, GLfloat y);
void set_object_static(GLfloat x, GLfloat y);

void init_transform();