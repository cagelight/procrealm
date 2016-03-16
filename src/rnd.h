#ifndef RND_H
#define RND_H

#include "com.h"

#define GLEW_NO_GLU
#include <GL/glew.h>

#define PROCMATH_UNIT GLfloat
#include "procmath.h"

//================================================================
//----------------------------------------------------------------
//============================= CORE =============================
// rnd_core.c

extern pthread_t rnd_thrd;
extern thrd_status_ta rnd_thrd_status;
extern char rnd_thrd_err [THRD_ERRLEN];
#define RNDSTAT atomic_load(&rnd_thrd_status)
#define RNDEXIT do {atomic_store(&rnd_thrd_status, THRD_STATUS_EXIT_NORMAL); pthread_exit(NULL);} while (0);
#define RNDEXER(err) do {strncpy(rnd_thrd_err, err, THRD_ERRLEN); atomic_store(&rnd_thrd_status, THRD_STATUS_EXIT_ERROR); pthread_exit(NULL);} while (0);

void rnd_init();

//================================================================
//----------------------------------------------------------------
//============================ SHADER ============================
// rnd_shader.c

extern GLuint shprog_basic;

void rnd_shader_init();

//================================================================
//----------------------------------------------------------------
//============================= UTIL =============================
// rnd_util.c

extern GLuint cube_up_vao;

void rnd_util_init();

//================================================================

#endif
