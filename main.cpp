#include <iostream>
#include <complex> 
#include <vector>
#include "Transmitter.h"
#include "Receiver.h"
#include "Wall.h"
#include "Simulation.h"
#include <SDL.h>
#include <stdio.h>
#include "Constants.h"
#include <chrono>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
double scale = 25.0;
int RENDER_X1 = SCREEN_WIDTH / 2;
int RENDER_Y1 = SCREEN_HEIGHT / 2;
int RENDER_X0 = SCREEN_WIDTH / 3.3;
int RENDER_Y0 = SCREEN_HEIGHT / 3;


void line(const Object& p1, const Object& p2) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + floor(scale * p1.x), RENDER_Y0 + floor(scale * p1.y), RENDER_X0 + floor(scale * p2.x), RENDER_Y0 + floor(scale * p2.y));
}
void point(const Object& p) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + floor(scale * (p.x - 0.5)), RENDER_Y0 + floor(scale * (p.y - 0.5)), RENDER_X0 + floor(scale * (p.x + 0.5)), RENDER_Y0 + floor(scale * (p.y + 0.5)));
    SDL_RenderDrawLine(renderer, RENDER_X0 + floor(scale * (p.x - 0.5)), RENDER_Y0 + floor(scale * (p.y + 0.5)), RENDER_X0 + floor(scale * (p.x + 0.5)), RENDER_Y0 + floor(scale * (p.y - 0.5)));
}
void area(const Object& p, double power) {
    SDL_Rect r = { RENDER_X0 + floor(scale * (p.x - 0.4)), RENDER_Y0 + floor(scale * (p.y - 0.4)), floor(0.8*scale), floor(0.8*scale) };
    SDL_SetRenderDrawColor(renderer, power*255.0, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &r);
}
bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        window = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        }
    }
    return success;
}
void close() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;
    SDL_Quit();
}

void field(complex<double>& field, complex<double>& coeff, double d, bool isLocal) {
    complex<double> result = coeff * sqrt(60 * ptxgtx) * exp(-j * beta * d) / d;
    if (isLocal) {
        result = norm(result);
    }
    //cout << "champ = " << result << endl;
    field = field + result;
}

void transmission(Wall walls[], const int i, Object& p1, Object& p2, complex<double>& coeff) {
    complex<double> result = walls[i].transmission(p1, p2);
    coeff = coeff * result;
    //cout << "Coeff de transmission du mur " << i << " = " << result << endl;
}

void reflection(Wall walls[], const int i, Object& p1, Object& p2, complex<double>& coeff) {
    complex<double> result = walls[i].reflection(p1, p2);
    coeff = coeff * result;
    //cout << "Coeff de reflexion du mur " << i << " = " << result << endl;
}


long double power(const complex<double>& field, bool isLocal) {
    if (isLocal) {
        return lambda * lambda / (8 * M_PI * M_PI * Ra) * real(field);
    }
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * norm(field);
}
double computation(Wall walls[], int n_walls, Transmitter& emetteur, Receiver& recepteur, bool isLocal) {
    complex<double> coeff(1.0, 0.0);
    Object hello;
    for (int i = 0; i < n_walls; i++) {
        if (walls[i].inter(emetteur, recepteur, hello)) {
            transmission(walls, i, emetteur, recepteur, coeff);
        }
    }
    complex<double> champ = 0;
    field(champ, coeff, (recepteur - emetteur).norm(), isLocal);

    for (int i = 0; i < n_walls; i++) {
        Object reflex1;
        Object image1 = walls[i].image(emetteur);
        coeff = complex<double>(1.0, 0.0);
        if (walls[i].inter(image1, recepteur, reflex1)) {
            reflection(walls, i, image1, recepteur, coeff);
            for (int j = 0; j < n_walls; j++) {
                if (i != j && walls[j].inter(reflex1, recepteur, hello)) {
                    transmission(walls, j, reflex1, recepteur, coeff);
                }
            }
            for (int j = 0; j < n_walls; j++) {
                if (i != j && walls[j].inter(emetteur, reflex1, hello)) {
                    transmission(walls, j, emetteur, reflex1, coeff);
                }
            }
            field(champ, coeff, (recepteur - image1).norm(), isLocal);
        }
        for (int j = 0; j < n_walls; j++) {
            if (j != i) {
                Object reflex20;
                Object reflex21;
                coeff = complex<double>(1.0, 0.0);
                Object image2 = walls[j].image(image1);
                if (walls[j].inter(image2, recepteur, reflex20) && walls[i].inter(image1, reflex20, reflex21)) {
                    reflection(walls, j, image2, recepteur, coeff);
                    reflection(walls, i, image1, reflex20, coeff);
                    for (int k = 0; k < n_walls; k++) {
                        if (k != j && walls[k].inter(reflex20, recepteur, hello)) {
                            transmission(walls, k, reflex20, recepteur, coeff);
                        }
                    }
                    for (int k = 0; k < n_walls; k++) {
                        if (k != j && k != i && walls[k].inter(reflex21, reflex20, hello)) {
                            transmission(walls, k, reflex21, reflex20, coeff);
                        }
                    }
                    for (int k = 0; k < n_walls; k++) {
                        if (k != i && walls[k].inter(emetteur, reflex21, hello)) {
                            transmission(walls, k, emetteur, reflex21, coeff);
                        }
                    }
                    field(champ, coeff, (recepteur - image2).norm(), isLocal);
                }
                for (int k = 0; k < n_walls; k++) {
                    if (k != j) {
                        coeff = complex<double>(1.0, 0.0);
                        Object image3 = walls[k].image(image2);
                        Object reflex30;
                        Object reflex31;
                        Object reflex32;
                        if (walls[k].inter(image3, recepteur, reflex30) && walls[j].inter(image2, reflex30, reflex31) && walls[i].inter(image1, reflex31, reflex32)) {
                            reflection(walls, k, image3, recepteur, coeff);
                            reflection(walls, j, image2, reflex30, coeff);
                            reflection(walls, i, image1, reflex31, coeff);
                            for (int l = 0; l < n_walls; l++) {
                                if (k != l && walls[l].inter(reflex30, recepteur, hello)) {
                                    transmission(walls, l, reflex30, recepteur, coeff);
                                }
                            }
                            for (int l = 0; l < n_walls; l++) {
                                if (k != l && j != l && walls[l].inter(reflex31, reflex30, hello)) {
                                    transmission(walls, l, reflex31, reflex30, coeff);
                                }
                            }
                            for (int l = 0; l < n_walls; l++) {
                                if (j != l && i != l && walls[l].inter(reflex32, reflex31, hello)) {
                                    transmission(walls, l, reflex32, reflex31, coeff);
                                }
                            }
                            for (int l = 0; l < n_walls; l++) {
                                if (i != l && walls[l].inter(emetteur, reflex32, hello)) {
                                    transmission(walls, l, emetteur, reflex32, coeff);
                                }
                            }
                            field(champ, coeff, (recepteur - image3).norm(), isLocal);
                        }
                    }
                }
            }
        }
    }
    return power(champ, isLocal);
}

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << endl;
    if (!init()) {
        printf("Failed to initialize !\n");
    }
    else {
        bool quit = false;
        SDL_Event e;
        auto start = chrono::high_resolution_clock::now();
        
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        double powers[44][8];
        Receiver receivers[44][8];
        double powerReference = 0;
        for (int i = 0; i < 44; i++) {
            for (int j = 0; j < 8; j++) {
                receivers[i][j] = Receiver(-11.5 + i, 10.5 + j);
                powers[i][j] = computation(wallsPlane, n_walls, emetteur2, Receiver((double)(i) / 2, 15), true);
                powerReference = powers[i][j] > powerReference ? powers[i][j] : powerReference;
            }
        }
        for (int i = 0; i < 44; i++) {
            for (int j = 0; j < 8; j++) {
                area(receivers[i][j], powers[i][j]/powerReference);
            }
        }


        for (int i = 0; i < *(&wallsPlane + 1) - wallsPlane; i++) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            line(wallsPlane[i].X, wallsPlane[i].X2);
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        point(emetteur1);
        point(emetteur2);

        double powerTP8 = computation(walls, 3, emetteur, recepteur, false);
        cout << "Power TP 8 : " << powerTP8 << endl;
        SDL_RenderPresent(renderer);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        std::cout << "Duration (ms) : " << duration.count() << endl;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    }
    close();
    return 0;
}