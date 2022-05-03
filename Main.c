#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_running = false;

void print(const char* message) {
	fprintf(stderr, message);
}

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		print("Error Initializing SDL.\n");
		return false;
	}

	// create an SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS );
	
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

void setup() {

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

void render() {

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer); // Update the screen with any rendering performed since the previous call (Backbuffer)
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
	return 0;
}