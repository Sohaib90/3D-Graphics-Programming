#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6*2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

/* Defines a struct for dynamic size meshes with an array of vertices and faces */
typedef struct {
	vec3_t* vertices;
	face_t* faces;
	vec3_t rotation; // rotation of the mesh
} mesh_t;

extern mesh_t mesh; // global variable mesh which will be used everywhere
void load_mesh_cube_data(void);

#endif
