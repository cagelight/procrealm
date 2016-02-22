#ifdef XKEYSET_STRUCTDEF
#define XFUNC(keychar, glfwenum) inp_keystate_t keychar;
#endif

#ifdef XKEYSET_INIT
#define XFUNC(keychar, glfwenum) kst->keychar = INP_KEYINACTIVE;
#endif

#ifdef XKEYSET_UPD
#define XFUNC(keychar, glfwenum) inp_keyupd(&kst->keychar, glfwenum);
#endif

XFUNC(w, GLFW_KEY_W)
XFUNC(a, GLFW_KEY_A)
XFUNC(s, GLFW_KEY_S)
XFUNC(d, GLFW_KEY_D)
XFUNC(esc, GLFW_KEY_ESCAPE)

#undef XFUNC