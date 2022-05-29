#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "stdlib.h"

triangle_t* triangles_to_render = NULL;

bool is_running = true;
int previous_frame_time = 0;

float fov_factor = 640; // to scale pixels in the screen

vec3_t camera_pos = {.x = 0, .y = 0, .z = -5}; // setting camera at (0,0,-5)
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 }; // for degree of freedom (to increase/decrease)

void setup(void) {
	color_buffer = malloc(sizeof(uint32_t) * WIN_WIDTH * WIN_HEIGHT);

	if (!color_buffer) {
		print("Error allocating the demanded memory space using mallooc(). \n");
	}

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture (
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIN_WIDTH, 
		WIN_HEIGHT
	);
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT: // the x button on the top right
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
			break;
		}
	}
}

/* function recieves 3D vector and return a 2D projected point */
vec2_t naive_ortho_project(const vec3_t point) {
	vec2_t projected_point = { .x = fov_factor * point.x, .y = fov_factor * point.y };
	return projected_point;
}	

/* function for the perspective projection of 3D points into 2D*/
vec2_t perspective_project(const vec3_t point) {
	vec2_t pers_proj_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return pers_proj_point;
}


void update() {
	/* Use of frame time wait for consistent animation */
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	//previous_frame_time = SDL_GetTicks();

	int current_time = SDL_GetTicks();
	float delta_time = (current_time - previous_frame_time) / 1000.0f;
	previous_frame_time = current_time;

	// initialize the array of triangles to render
	triangles_to_render = NULL;

	/*
		apply a linear transformation before projecting
		This can be rotation, translation or scale
	*/
	float rot = (float)rand() / (float)(RAND_MAX / 1.0f);
	cube_rotation.x += rot * delta_time;
	cube_rotation.y += rot * delta_time;
	cube_rotation.z += rot * delta_time;

	for (size_t i = 0; i < N_MESH_FACES; i++)
	{
		face_t current_mesh_face = mesh_faces[i];
		vec3_t face_vertices[3];
		// index is from 1->n so (-1) should be done
		face_vertices[0] = mesh_vertices[current_mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[current_mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[current_mesh_face.c - 1];
		
		triangle_t projected_triangle;
		// loop all three vertices of this current face and apply transformations
		for (size_t j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);
			
			// translate the vertex away from the camera in the z axis
			transformed_vertex.z -= camera_pos.z;
			
			// perspective projection
			vec2_t projected_point = perspective_project(transformed_vertex);
			
			//scale and translte the point to the middle of the screen
			projected_point.x += WIN_WIDTH / 2;
			projected_point.y += WIN_HEIGHT / 2;
			projected_triangle.points[j] = projected_point;
		}

		// storing the actual coordinated of the vertices of each face
		array_push(triangles_to_render, projected_triangle);
	}
}

void render() {

	//draw_grid(0xFFFFFF, 50);
	//draw_rect(WIN_WIDTH/2 - 250 + move_x, WIN_HEIGHT/2, 500, 100, 0x0085ca);
	//render_string("HELLO ALL!", WIN_WIDTH/2 - 250, WIN_HEIGHT / 2 - 250, 250, 100, 0x0085ca, "bold");
	//draw_pixel(20, 20, 0xFFFFFF00);
	
	// Render projected triangles
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++){
		triangle_t current_triangle = triangles_to_render[i];
		draw_rect(current_triangle.points[0].x,
			current_triangle.points[0].y,
			3, 3, 0xFFFFFF00);
		draw_rect(current_triangle.points[1].x,
			current_triangle.points[1].y,
			3, 3, 0xFFFFFF00);
		draw_rect(current_triangle.points[2].x,
			current_triangle.points[2].y,
			3, 3, 0xFFFFFF00);

		draw_triangle(current_triangle.points[0].x, current_triangle.points[0].y,
			current_triangle.points[1].x, current_triangle.points[1].y,
			current_triangle.points[2].x, current_triangle.points[2].y, 0xFF00FF00);
	}

	// clear the array of triangles I render (clean up)
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0x000000);

	SDL_RenderPresent(renderer); // Update the screen with any rendering performed since the previous call (Backbuffer)

}

// For SDL to work, the main function needs to have two arguments 
int main(int argc, char* argv[])
{
	
	is_running = initialize_window();

	setup();

	while (is_running) {
		/* game loop: one frame execution */
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}