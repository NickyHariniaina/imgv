#include <SDL2/SDL.h>
#include <stdio.h>

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init video");
        return -1;
    }
    return 0;
}
