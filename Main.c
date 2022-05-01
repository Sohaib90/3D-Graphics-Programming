#include <stdio.h>
#include <SDL.h>

// For SDL to work, the main function needs to have two arguments 
int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("Hello World!");
	return 0;
}