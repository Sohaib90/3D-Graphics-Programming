#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int WIN_HEIGHT = 0;
int WIN_WIDTH = 0;
bool is_running = false;
uint32_t* color_buffer = NULL;

void print(const char* message) {
	fprintf(stderr, message);
}

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		print("Error Initializing SDL.\n");
		return false;
	}

	// Query SDL to find the maximum resolution of the monitor/window being used
	// This converts the full screen referred to as fake full screen
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	WIN_HEIGHT = display_mode.h;
	WIN_WIDTH = display_mode.w;

	// create an SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH,
		WIN_HEIGHT,
		SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	if (!window) {
		print("Error Initializing SDL window. \n");
		return false;
	}

	// attach a renderer with the window 
	renderer = SDL_CreateRenderer(window,-1, 0);

	if (!renderer) {
		print("Error creating SDL renderer. \n");
		return false;
	}

	return true;
}

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


void update() {

}

void clear_color_buffer(uint32_t color) {
	for (size_t y = 0; y < WIN_HEIGHT; y++)
	{
		for (size_t x = 0; x < WIN_WIDTH; x++)
		{
			color_buffer[WIN_WIDTH * y + x] = color;
		}
	}
}

void draw_grid(uint32_t background, uint32_t line_color, int offset) {
	
	clear_color_buffer(background);

	for (size_t i = 0, width = WIN_WIDTH; i < width; i+= offset)
	{
		for (size_t j = 0, height = WIN_HEIGHT; j < height; j++)
		{
			color_buffer[WIN_WIDTH * j + i] = line_color;
		}
	}


	for (size_t i = 0, height = WIN_HEIGHT; i < height; i += offset)
	{
		for (size_t j = 0, width = WIN_WIDTH; j < width; j++)
		{
			if (color_buffer[WIN_WIDTH * i + j] != line_color)
				color_buffer[WIN_WIDTH * i + j] = line_color;
		}
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color, uint32_t background) {
	clear_color_buffer(background);

	for (size_t i = y; i < y + height; i++)
	{
		for (size_t j = x; j < x + width; j++)
		{
			color_buffer[WIN_WIDTH * i + j] = color;

		}
	}
}

void render_color_buffer() {

	// Bind te texture with color buffer
	if (SDL_UpdateTexture(
		color_buffer_texture,
		NULL, 
		color_buffer, 
		(int)(WIN_WIDTH * sizeof(uint32_t))
	)) 
	{
		print("Failed to UpdateTexture.\n");
	}

	// Copy the texture to the renderer
	if (SDL_RenderCopy(
		renderer,
		color_buffer_texture,
		NULL,
		NULL
	)) 
	{
		print("Failed to Render copy the texture to the renderer.\n");
	}
}

void render() {

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw_grid(0x000000, 0xFFFFFF, 50);
	draw_rect(WIN_WIDTH/2 - 250, WIN_HEIGHT/2, 500, 100, 0x0085ca, 0x000000);
	render_color_buffer();

	SDL_RenderPresent(renderer); // Update the screen with any rendering performed since the previous call (Backbuffer)

}

void destroy_window(void) {

	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


// For SDL to work, the main function needs to have two arguments 
int main(int argc, char* argv[])
{
	
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}