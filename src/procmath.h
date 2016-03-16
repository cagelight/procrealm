#ifndef PROCMATH_H
#define PROCMATH_H

#include <math.h>

//================================================================
//----------------------------------------------------------------
//=========================== VECTOR 2 ===========================

typedef double vec2d[2];
typedef float vec2f[2];

#define vec2_set(V, X, Y) do { \
	V[0] = X; \
	V[1] = Y; \
} while (0);

#define vec2_add(A, B, OUT) do { \
	OUT[0] = A[0] + B[0]; \
	OUT[1] = A[1] + B[1]; \
} while (0);

#define vec2_sub(A, B, OUT) do { \
	OUT[0] = A[0] - B[0]; \
	OUT[1] = A[1] - B[1]; \
} while (0);

#define vec2_mult(A, MAG, OUT) do { \
	OUT[0] = A[0] * MAG; \
	OUT[1] = A[1] * MAG; \
} while (0);

#define vec2_div(A, MAG, OUT) vec2_mult(A, 1.0/MAG, OUT)

#define vec2_dot(A, B) (A[0]*B[0] + A[1]*B[1])

//================================================================
//----------------------------------------------------------------
//=========================== VECTOR 3 ===========================

typedef double vec3d[3];
typedef float vec3f[3];

#define vec3_set(V, X, Y, Z) do { \
	V[0] = X; \
	V[1] = Y; \
	V[2] = Z; \
} while (0);

#define vec3_add(A, B, OUT) do { \
	OUT[0] = A[0] + B[0]; \
	OUT[1] = A[1] + B[1]; \
	OUT[2] = A[2] + B[2]; \
} while (0);

#define vec3_sub(A, B, OUT) do { \
	OUT[0] = A[0] - B[0]; \
	OUT[1] = A[1] - B[1]; \
	OUT[2] = A[2] - B[2]; \
} while (0);

#define vec3_mult(A, MAG, OUT) do { \
	OUT[0] = A[0] * MAG; \
	OUT[1] = A[1] * MAG; \
	OUT[2] = A[2] * MAG; \
} while (0);

#define vec3_div(A, MAG, OUT) vec3_mult(A, 1.0/MAG, OUT)

#define vec3_mag(A) sqrt(A[0]*A[0] + A[1]*A[1] + A[2]*A[2])

#define vec3_norm(A, OUT) do { \
	double _len = vec3_mag(A); \
	OUT[0] = A[0] / _len; \
	OUT[1] = A[1] / _len; \
	OUT[2] = A[2] / _len; \
} while (0);

#define vec3_dot(A, B) (A[0]*B[0] + A[1]*B[1] + A[2]*B[2])

#define vec3_cross(A, B, OUT) do { \
	OUT[0] = A[1]*B[2] - A[2]*B[1];\
	OUT[1] = A[2]*B[0] - A[0]*B[2];\
	OUT[2] = A[0]*B[1] - A[1]*B[0];\
} while (0);

//================================================================
//----------------------------------------------------------------
//=========================== VECTOR 4 ===========================

typedef double vec4d[4];
typedef float vec4f[4];

#define vec4_set(V, X, Y, Z, W) do { \
	V[0] = X; \
	V[1] = Y; \
	V[2] = Z; \
	V[3] = W; \
} while (0);

#define vec4_add(A, B, OUT) do { \
	OUT[0] = A[0] + B[0]; \
	OUT[1] = A[1] + B[1]; \
	OUT[2] = A[2] + B[2]; \
	OUT[3] = A[3] + B[4]; \
} while (0);

#define vec4_sub(A, B, OUT) do { \
	OUT[0] = A[0] - B[0]; \
	OUT[1] = A[1] - B[1]; \
	OUT[2] = A[2] - B[2]; \
	OUT[3] = A[3] - B[4]; \
} while (0);

#define vec4_mult(A, MAG, OUT) do { \
	OUT[0] = A[0] * MAG; \
	OUT[1] = A[1] * MAG; \
	OUT[2] = A[2] * MAG; \
	OUT[3] = A[3] * MAG; \
} while (0);

#define vec4_div(A, MAG, OUT) vec4_mult(A, 1.0/MAG, OUT)

#define vec4_dot(A, B) (A[0]*B[0] + A[1]*B[1] + A[2]*B[2] + A[3]*B[3])

//================================================================
//----------------------------------------------------------------
//=========================== MATRIX 4 ===========================

typedef double mat4d[4][4];
typedef float mat4f[4][4];

#define mat4_ident(M) do { \
	memset(&M, 0, sizeof(M)); \
	M[0][0] = 1; \
	M[1][1] = 1; \
	M[2][2] = 1; \
	M[3][3] = 1; \
} while (0);

#define mat4_mult_mat4(A, B, OUT) do { \
	OUT[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];\
	OUT[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];\
	OUT[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];\
	OUT[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];\
	\
	OUT[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];\
	OUT[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];\
	OUT[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];\
	OUT[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];\
	\
	OUT[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];\
	OUT[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];\
	OUT[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];\
	OUT[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];\
	\
	OUT[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];\
	OUT[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];\
	OUT[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];\
	OUT[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];\
} while (0);

#define mat4_lookat(EYE, CENTER, UP, M) do { \
	vec3d F, S, U; \
	vec3_sub(CENTER, EYE, F); \
	vec3_norm(F, F); \
	vec3_cross(UP, F, S); \
	vec3_norm(S, S); \
	vec3_cross(F, S, U); \
	mat4_ident(M); \
	M[0][0] = S[0]; \
	M[1][0] = S[1]; \
	M[2][0] = S[2]; \
	M[0][1] = U[0]; \
	M[1][1] = U[1]; \
	M[2][1] = U[2]; \
	M[0][2] = F[0]; \
	M[1][2] = F[1]; \
	M[2][2] = F[2]; \
	M[3][0] = -vec3_dot(S, EYE); \
	M[3][1] = -vec3_dot(U, EYE); \
	M[3][2] = -vec3_dot(F, EYE); \
} while (0);

#define mat4_project_perspective(FOV, WIDTH, HEIGHT, NEAR, FAR, OUT) do { \
	double H = cos(0.5 * FOV) / sin(0.5 * FOV); \
	double W = H * (HEIGHT / WIDTH); \
	memset(&OUT, 0, sizeof(OUT)); \
	OUT[0][0] = W; \
	OUT[1][1] = H; \
	OUT[2][3] = 1.0; \
	OUT[2][2] = (FAR + NEAR) / (FAR - NEAR); \
	OUT[3][2] = -(2 * FAR * NEAR) / (FAR - NEAR); \
} while (0);

//================================================================

#endif
