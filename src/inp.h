#ifndef INP_H
#define INP_H

#include "com.h"

extern pthread_t inp_thrd;
extern thrd_status_ta inp_thrd_status;
extern char inp_thrd_err [THRD_ERRLEN];
#define INPSTAT atomic_load(&inp_thrd_status)
#define INPEXIT atomic_store(&inp_thrd_status, THRD_STATUS_EXIT_NORMAL); pthread_exit(NULL);
#define INPEXER(err) strncpy(inp_thrd_err, err, THRD_ERRLEN); atomic_store(&inp_thrd_status, THRD_STATUS_EXIT_ERROR); pthread_exit(NULL);

enum inp_keystate_e {
	INP_KEYINACTIVE = 1<<0,
	INP_KEYPRESSED = 1<<1,
	INP_KEYRELEASED = 1<<2,
	INP_KEYHELD = 1<<3
};

typedef int_fast8_t inp_keystate_t;

typedef struct inp_keystuff_s {
	char const * name;
	int glfwkey;
	inp_keystate_t state;
	double time;
} inp_keystuff_t;

typedef struct inp_keyset_s {
#define XKEYSET_STRUCTDEF
#include "inp_xkeyset.h"
#undef XKEYSET_STRUCTDEF
} inp_keyset_t;

#define KEYTIME(keyset, keychar) keyset->th_##keychar 

void inp_keyset_init(inp_keyset_t *);
void inp_keyset_ipd(inp_keyset_t *, timekeeper_t *);

int inp_init();

#endif
