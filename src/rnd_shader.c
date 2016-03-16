#include "rnd.h"

GLuint shprog_basic;
static GLuint sh_basic_vertex;
static GLuint sh_basic_fragment;

static char const * sh_basic_vertex_source = 
" \
#version 450 core \n \
layout(location = 0) in vec3 pos; \
uniform mat4 trans;\
void main () { \
	gl_Position = trans * vec4(pos, 1); \
} \
";

static char const * sh_basic_fragment_source = 
" \
#version 450 core \n \
layout(location = 0) out vec4 color; \
void main () { \
	color = vec4(1, 1, 0, 1); \
} \
";

static bool rnd_compile_shader(GLuint shader, char const * src) {
	GLint success;
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar * info_log = (GLchar *)malloc(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, info_log);
		com_printf_error("shader failed to compile: \n%s", info_log);
		free(info_log);
		return false;
	}
	return true;
}

static bool rnd_link_shader_program(GLuint shprog) {
	GLint success;
	glLinkProgram(shprog);
	glGetProgramiv(shprog, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(shprog, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar * info_log = (GLchar *)malloc(maxLength);
		glGetProgramInfoLog(shprog, maxLength, &maxLength, info_log);
		com_printf_error("program failed to link: \n%s", info_log);
		free(info_log);
		return false;
	}
	return true;
}

void rnd_shader_init() {
	sh_basic_vertex = glCreateShader(GL_VERTEX_SHADER);
	sh_basic_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	shprog_basic = glCreateProgram();
	
	if (!rnd_compile_shader(sh_basic_vertex, sh_basic_vertex_source)) RNDEXER("basic vertex shader failed to compile");
	if (!rnd_compile_shader(sh_basic_fragment, sh_basic_fragment_source)) RNDEXER("basic fragment shader failed to compile");
	glAttachShader(shprog_basic, sh_basic_vertex);
	glAttachShader(shprog_basic, sh_basic_fragment);
	if (!rnd_link_shader_program(shprog_basic)) RNDEXER("basic shader program failed to link");
}
