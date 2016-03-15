#include "rnd.h"

GLuint cube_up_vao;
static GLuint cube_up_vbo_verts;
static float cube_up_verts [] = {
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};

void rnd_util_init() {
	glCreateVertexArrays(1, &cube_up_vao);
	glCreateBuffers(1, &cube_up_vbo_verts);
	glNamedBufferData(cube_up_vbo_verts, sizeof(cube_up_verts), cube_up_verts, GL_STATIC_DRAW);
	glVertexArrayAttribBinding(cube_up_vao, 0, 0);
	glVertexArrayVertexBuffer(cube_up_vao, 0, cube_up_vbo_verts, 0, 12);
	glEnableVertexArrayAttrib(cube_up_vao, 0);
	glVertexArrayAttribFormat(cube_up_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	/*
	glCreateVertexArrays(1, &test_box_vao);
	glCreateBuffers(1, &test_box_vbo_verts);
	glNamedBufferData(test_box_vbo_verts, sizeof(test_box_verts), test_box_verts, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(test_box_vao, 0, 0);
	glVertexArrayVertexBuffer(test_box_vao, 0, test_box_vbo_verts, 0, 108);
	glEnableVertexArrayAttrib(test_box_vao, 0);
	glVertexArrayAttribFormat(test_box_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	*/
}
