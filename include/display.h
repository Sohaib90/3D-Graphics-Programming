#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;

extern int WIN_HEIGHT;
extern int WIN_WIDTH;
extern bool is_running;
extern uint32_t* color_buffer;

// Display function declarations
bool initialize_window(void);
void draw_grid(uint32_t line_color, int offset);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(float x0, float y0, float x1, float y1, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void render_color_buffer();
void clear_color_buffer(uint32_t color);
void destroy_window(void);
void print(const char* message);
void render_string(const char* message, const int x, const int y, const int height, const int width, 
				   const uint32_t color, const char* font_style);


#endif
