#include "display.h"

SDL_Window*  window = NULL ;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int WIN_HEIGHT = 600;
int WIN_WIDTH = 800;
uint32_t* color_buffer = NULL;

void print(const char* message) {
	fprintf(stderr, message);
}

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		print("Error creating SDL renderer. \n");
		return false;
	}

	return true;
}

void clear_color_buffer(uint32_t color) {
	for (uint32_t y = 0; y < WIN_HEIGHT; y++)
	{
		for (size_t x = 0; x < WIN_WIDTH; x++)
		{
			color_buffer[WIN_WIDTH * y + x] = color;
		}
	}
}

void draw_grid(uint32_t line_color, int offset) {

	for (size_t i = 0, width = WIN_WIDTH; i < width; i += offset)
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

void draw_rect(int x, int y, int width, int height, uint32_t color) {

	for (size_t i = y; i < y + height; i++){
		for (size_t j = x; j < x + width; j++)
			draw_pixel(j, i, color);
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		color_buffer[WIN_WIDTH * y + x] = color;
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	// Using DDA algorithm to draw a line 
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;

	int side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float curr_x = x0;
	float curr_y = y0;

	for (size_t i = 0; i <= side_length; i++)
	{
		draw_pixel(round(curr_x), round(curr_y), color);

		curr_x += x_inc;
		curr_y += y_inc;
	}
}

void draw_bres_line(int x1, int y1, int x2, int y2, uint32_t color) {
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x0, y0, x2, y2, color);
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

void destroy_window(void) {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void render_string(const char* message, 
				   const int x, const int y, 
				   const int height, const int width, 
				   const uint32_t color, const char* font_style) {
	/* 
		Given a string, its location and font, render the text using buffer 
	*/
	int pos_x = x;

	for (size_t i = 0; i < strlen(message); i++)
	{

		if (message[i] == 'H') {

			if (font_style != "bold") {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;

				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						color_buffer[(WIN_WIDTH * (y + (height / 2) + j)) + (pos_x + k)] = color;
					}
				}

				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
			}
			else {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;

				for (size_t j = 0; j < 6; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						color_buffer[(WIN_WIDTH * (y + (height / 2) + j)) + (pos_x + k)] = color;
					}
				}

				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
			}

			pos_x += 10;

		}
		else if (message[i] == 'E')
		{
			if (font_style != "bold") {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;

				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						// top E line
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						// middle E line
						color_buffer[(WIN_WIDTH * (y + (height / 2) + j)) + (pos_x + k)] = color;
						// last E line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}

				pos_x += 20;
			}
			else {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;

				for (size_t j = 0; j < 6; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						// top E line
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						// middle E line
						color_buffer[(WIN_WIDTH * (y + (height / 2) + j)) + (pos_x + k)] = color;
						// last E line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}

				pos_x += 20;
			}

			pos_x += 10;
		}
		else if (message[i] == 'L') {
			if (font_style != "bold") {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						// last L line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}

				pos_x += 20;

			}
			else {
				for (size_t j = y; j < y + height; j++) {
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
				for (size_t j = 0; j < 6; j++) {
					for (size_t k = 0; k < 20; k++) {
						// last L line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}
				pos_x += 20;
			}
			pos_x += 10;
		}
		else if (message[i] == 'O') {
			if (font_style != "bold") {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						// top E line
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						// last E line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}
				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
			}
			else {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
				for (size_t j = 0; j < 6; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						// top E line
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						// last E line
						color_buffer[(WIN_WIDTH * (y + (height - 1) - j)) + (pos_x + k)] = color;
					}
				}
				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
			}

			pos_x += 10;
		}
		else if (message[i] == ' ') {
		if (font_style != "bold") {
			pos_x += 20;
		}
		else {
			pos_x += 30;
		}
		pos_x += 10;
		}
		else if (message[i] == 'A') {
			if (font_style != "bold") {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
				for (size_t j = 0; j < 3; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						color_buffer[(WIN_WIDTH * (y + (height / 2) - j)) + (pos_x + k)] = color;
					}
				}
				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
				}
				pos_x += 3;
			}
			else {
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
				for (size_t j = 0; j < 6; j++)
				{
					for (size_t k = 0; k < 20; k++)
					{
						color_buffer[(WIN_WIDTH * (y + j)) + (pos_x + k)] = color;
						color_buffer[(WIN_WIDTH * (y + (height / 2) - j)) + (pos_x + k)] = color;
					}
				}
				pos_x += 20;
				for (size_t j = y; j < y + height; j++)
				{
					color_buffer[WIN_WIDTH * j + (pos_x)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 1)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 2)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 3)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 4)] = color;
					color_buffer[WIN_WIDTH * j + (pos_x + 5)] = color;
				}
				pos_x += 6;
			}

			pos_x += 10;
		}
	}

}

