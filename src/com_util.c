#include "com.h"

#include <stdarg.h>
#include <stdio.h>

static pthread_mutex_t prntmut;
void com_print(char const * str) {
	pthread_mutex_lock(&prntmut);
	printf("%s", str);
	if (str[strlen(str)-1] != '\n') printf("\n");
	pthread_mutex_unlock(&prntmut);
}

void com_util_init() {
	pthread_mutex_init(&prntmut, NULL);
}
