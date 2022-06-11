#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef union {
	int face_num[3];
	struct {
		int a;
		int b;
		int c;
	};
}face_t;

typedef struct {
	vec2_t points[3];
}triangle_t;

#endif // !TRIANGLE_H

