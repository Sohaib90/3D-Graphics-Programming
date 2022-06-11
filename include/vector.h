#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
} vec2_t;

typedef union
{
	float points[3];
	struct {
		float x;
		float y;
		float z;
	};
}vec3_t;

vec3_t vec3_rotate_x(const vec3_t v, float angle);
vec3_t vec3_rotate_y(const vec3_t v, float angle);
vec3_t vec3_rotate_z(const vec3_t v, float angle);

#endif // !VECTOR_H

