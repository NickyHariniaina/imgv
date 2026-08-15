#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char *window_title;
    int pos_x;
    int pos_y;
    int height;
    int width;
    int flag;
} WindowProp;

static SDL_Window *create_window(WindowProp window_prop) {
    return SDL_CreateWindow(window_prop.window_title, window_prop.pos_x,
                            window_prop.pos_y, window_prop.width,
                            window_prop.height, window_prop.flag);
}

static SDL_Renderer *create_renderer(SDL_Window *window) {
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Not enough arguments.");
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to init video");
        return -1;
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) !=
        (IMG_INIT_PNG | IMG_INIT_JPG)) {
        printf("Cannot initialize SDL_image");
        return -1;
    }

    SDL_Surface *surface = IMG_Load(argv[1]);

    if (surface == NULL) {
        printf("Cannot load image");
        return -1;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    float scale_x = (float)display_mode.w / surface->w;
    float scale_y = (float)display_mode.h / surface->h;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;

    if (scale > 1.0f) scale = 1.0f;

    WindowProp window_prop;
    window_prop.pos_x = SDL_WINDOWPOS_CENTERED;
    window_prop.flag = 0;
    window_prop.pos_y = SDL_WINDOWPOS_CENTERED;
    window_prop.width = surface->w * scale;
    window_prop.height = surface->h * scale;
    window_prop.window_title = "new image";

    SDL_Window *window = create_window(window_prop);

    if (window == NULL) {
        printf("Couldn't create window");
        return -1;
    }

    SDL_Renderer *renderer = create_renderer(window);

    if (renderer == NULL) {
        printf("Could not create renderer");
        return -1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        // Here. I actually need to close some stuff but too lazy for now
        printf("Cannot create texture");
        return -1;
    }

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = window_prop.width;
    rect.h = window_prop.height;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(surface);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
