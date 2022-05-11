#include <iostream>
#include <complex> 
#include "Transmitter.h"
#include "Receiver.h"
#include "Wall.h"
#include <SDL.h>
#include <stdio.h>
#include "Constants.h"
#include <chrono>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
double scale = 4;
int RENDER_X0 = SCREEN_WIDTH / 2;
int RENDER_Y0 = SCREEN_HEIGHT / 2;

void line(Object p1, Object p2) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + scale * p1.x, RENDER_Y0 - scale * p1.y, RENDER_X0 + scale * p2.x, RENDER_Y0 - scale * p2.y);
    //SDL_RenderDrawLine(renderer, x1, - y1,  x2,  y2);
}
void point(Object p) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + scale * p.x - 2, RENDER_Y0 - scale * p.y - 2, RENDER_X0 + scale * p.x + 2, RENDER_Y0 - scale * p.y + 2);
    SDL_RenderDrawLine(renderer, RENDER_X0 + scale * p.x - 2, RENDER_Y0 - scale * p.y + 2, RENDER_X0 + scale * p.x + 2, RENDER_Y0 - scale * p.y - 2);
    //SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2 + scale * x, SCREEN_HEIGHT / 2 - scale * y);
}
void area(Object p) {
    SDL_Rect r = { RENDER_X0 + scale * p.x - 5, RENDER_Y0 - scale * p.y - 5, 10, 10 };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x88, 0xFF);
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

Transmitter emetteur(32, 10);  //initialisation d'un emetteur
Receiver recepteur(47, 65);    //initialisation d'un recepteur
Wall wall1(0, 0, 80, 90, thickness, epsR, sigma); //initialisation des murs
Wall wall2(0, 20, 80, 0, thickness, epsR, sigma);
Wall wall3(0, 80, 80, 0, thickness, epsR, sigma);
Wall walls[] = { wall1, wall2, wall3 };

complex <double> field(Object p[], int n, int index[], double d) {
    complex<double> coeff(1.0, 0.0);
    cout << "d = " << d << endl;
    complex<double> temp;
    if (n == 0) {
        temp = walls[1].transmission(walls[1].cos_i(emetteur, recepteur));
        coeff = coeff * temp;
        cout << "Coefficient de transmission = " << temp << endl;
    }
    else {
        temp = walls[1].transmission(walls[1].cos_i(emetteur, p[0]));
        coeff = coeff * temp;
        cout << "Coefficient de transmission = " << temp << endl;
        for (int i = 0; i < n - 1; i++) {
            temp = walls[index[i]].reflection(walls[index[i]].cos_i(p[i], p[i + 1]));
            coeff = coeff * temp;
            cout << "Coefficient de reflexion = " << temp << endl;
        }
        temp = walls[index[n-1]].reflection(walls[index[n-1]].cos_i(p[n-1], recepteur));
        coeff = coeff * temp;
        cout << "Coefficient de reflexion = " << temp << endl;
    }


    return coeff * sqrt(60 * ptxgtx) * exp(-j * beta * d) / d;
}

long double power(complex<double> Es [], int n) {
    complex<double> sum = 0;
    for (int i = 0; i < n; i++) {
        sum += Es[i];
    }
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * norm(sum);
}

long double local_power(double E) {
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * E;
}



int main(int argc, char* argv[]) {
    cout << "Hello, World!" << endl;

    Wall wallsPlane[] = {
        Wall(0, 10, 31, 0, thickness, epsR, sigma),
        Wall(31, 10, sqrt(2), 45, thickness, epsR, sigma),
        Wall(32, 11, 6, 90, thickness, epsR, sigma),
        Wall(32, 17, sqrt(2), 135, thickness, epsR, sigma),
        Wall(31, 18, 31, 180, thickness, epsR, sigma)
    };
    Object image1[3];
    Object image2[3][3];
    Object image3[3][3][3];

    complex<double> champs[100];
    int n_champs = 1;

    if (!init()) {
        printf("Failed to initialize !\n");
    }
    else {
        bool quit = false;
        SDL_Event e;
        auto start = chrono::high_resolution_clock::now();
        
        Object p[1] = {};
        int index[] = { 0 };
        champs[0] = field(p, 0, index, (recepteur - emetteur).norm());
        cout << "Pas de reflexion : E = " << champs[0] << endl;
        
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        line(recepteur, emetteur);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        point(recepteur);
        area(recepteur);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        point(emetteur);

        /*for (int i = 0; i < *(&wallsPlane + 1) - wallsPlane; i++) {
            line(wallsPlane[i].X, wallsPlane[i].X2);
        }*/

        for (int i = 0; i < 3; i++) {

            image1[i] = walls[i].image(emetteur);
            Object reflex1 = walls[i].refP(emetteur, recepteur);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            line(walls[i].X, walls[i].X2);

            if (walls[i].isReflected(recepteur, emetteur)) {
                cout << image1[i].x << endl;
                Object p1[] = { reflex1 };
                int index1[] = { i };
                champs[n_champs] = field(p1, 1, index1, (recepteur - image1[i]).norm());
                cout << "i = " << i << ", E = " << champs[n_champs] << endl;
                n_champs++;
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                line(recepteur, reflex1);
                line(emetteur, reflex1);
            }
            for (int j = 0; j < 3; j++) {
                if (j != i) {
                    image2[i][j] = walls[j].image(image1[i]);
                    Object reflex20 = walls[j].refP(recepteur, image1[i]);
                    Object reflex21 = walls[i].refP(emetteur, reflex20);

                    if (walls[j].isReflected(recepteur, reflex21) && walls[i].isReflected(reflex20, emetteur)) {
                        Object p2[] = {reflex21, reflex20};
                        int index2[] = { i, j };
                        champs[n_champs] = field(p2, 2, index2, (recepteur - image2[i][j]).norm());
                        cout << "i = " << i << ", j = " << j << ", E = " << champs[n_champs] << endl;
                        n_champs++;
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x88, 0x00, 0xFF);
                        line(emetteur, reflex21);
                        line(reflex21, reflex20);
                        line(reflex20, recepteur);
                    }
                    for (int k = 0; k < 3; k++) {
                        if (k != j) {
                            image3[i][j][k] = walls[k].image(image2[i][j]);
                            Object reflex30 = walls[k].refP(recepteur, image2[i][j]);
                            Object reflex31 = walls[j].refP(image1[i], reflex30);
                            Object reflex32 = walls[i].refP(reflex31, emetteur);
                            if (walls[k].isReflected(recepteur, reflex31) && walls[j].isReflected(reflex30, reflex32) && walls[i].isReflected(reflex31, emetteur)) {
                                Object p3[] = { reflex32, reflex31, reflex30};
                                int index3[] = { i, j, k };
                                champs[n_champs] = field(p3, 3, index3, (image3[i][j][k] - recepteur).norm());
                                cout << "i = " << i << ", j = " << j << ", k = " << k << ", E = " << champs[n_champs] << endl;
                                n_champs++;
                                cout << "i, j, k = " << i << ", " << j << ", " << k << endl;
                                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                                line(emetteur, reflex32);
                                line(reflex32, reflex31);
                                line(reflex31, reflex30);
                                line(reflex30, recepteur);
                            }
                        }
                    }
                }
            }
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "P = " << power(champs, n_champs) << endl;
        cout << "Duration (ms) : " << duration.count() << endl;
        SDL_RenderPresent(renderer);
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