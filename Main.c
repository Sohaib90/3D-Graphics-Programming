#include "display.h"
#include "vector.h"

#define N_POINTS (9*9*9)
vec3_t cube_points[N_POINTS]; // 9x9x9 cube [-1, 1]
bool is_running = true;

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

	int point_count = 0;

	// start loading array of vectors
	for (float x = -1; x <= 1; x+= 0.25){
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { .x = x, .y = y, .z = z };
				cube_points[point_count] = new_point;
				point_count++;
			}
		}
	}
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


void update() {

}

void render(int move_x) {

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	//draw_grid(0xFFFFFF, 50);
	//draw_rect(WIN_WIDTH/2 - 250 + move_x, WIN_HEIGHT/2, 500, 100, 0x0085ca);
	//render_string("HELLO ALL!", WIN_WIDTH/2 - 250, WIN_HEIGHT / 2 - 250, 250, 100, 0x0085ca, "bold");
	//draw_pixel(20, 20, 0xFFFFFF00);
	render_color_buffer();
	clear_color_buffer(0x000000);

	SDL_RenderPresent(renderer); // Update the screen with any rendering performed since the previous call (Backbuffer)

}

// For SDL to work, the main function needs to have two arguments 
int main(int argc, char* argv[])
{
	
	is_running = initialize_window();
	int move_x = 0;

	setup();

	while (is_running) {
		process_input();
		update();
		render(move_x);
		//move_x++;
	}

	destroy_window();
	return 0;
}