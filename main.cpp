#include <iostream>
#include <complex> 
#include "Transmitter.h"
#include "Receiver.h"
#include "Wall.h"
#include <SDL.h>
#include <stdio.h>
#include "Constants.h"

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

complex <double> champ(complex<double> coeffs[], int n, double d) {
    complex<double> coeff(1.0, 0.0);
    cout << "d = " << d << endl;
    for (int i = 0; i < n; i++) {
        coeff = coeff * coeffs[i];
        cout << "Coeff = " << coeff << endl;
    }
    return coeff * sqrt(60 * ptxgtx) * exp(-j * beta * d) / d;
}

long double power(complex<double> Es [], int n) {
    complex<double> sum = 0;
    for (int i = 0; i < n; i++) {
        sum += Es[i];
        cout << "on ajoute" << endl;
    }
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * norm(sum);
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
        window = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

double scale = 4;

void line(Object p1, Object p2) {
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * p1.x, SCREEN_HEIGHT / 2 - scale * p1.y, SCREEN_WIDTH / 2 + scale * p2.x, SCREEN_HEIGHT / 2 - scale * p2.y);
    //SDL_RenderDrawLine(renderer, x1, - y1,  x2,  y2);
}

void point(Object p) {
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * p.x - 2, SCREEN_HEIGHT / 2 - scale * p.y - 2, SCREEN_WIDTH / 2 + scale * p.x + 2, SCREEN_HEIGHT / 2 - scale * p.y + 2);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * p.x - 2, SCREEN_HEIGHT / 2 - scale * p.y + 2, SCREEN_WIDTH / 2 + scale * p.x + 2, SCREEN_HEIGHT / 2 - scale * p.y - 2);
    //SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2 + scale * x, SCREEN_HEIGHT / 2 - scale * y);
}


int main(int argc, char* argv[]) {
    
    cout << "Hello, World!" << endl;
    Transmitter emetteur(32, 10);  //initialisation d'un emetteur
    Receiver recepteur(47, 65);    //initialisation d'un recepteur
    Wall wall1 = Wall(0, 0, 80, 90, thickness, epsR, sigma); //initialisation des murs
    Wall wall2 = Wall(0, 20, 80, 0, thickness, epsR, sigma);
    Wall wall3 = Wall(0, 80, 80, 0, thickness, epsR, sigma);
    Wall walls[] = {wall1, wall2, wall3};
    
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

    Object reflex1[3];
    Object reflex2[3][3][2];
    Object reflex3[3][3][3][3];


    
    for (int i = 0; i < 3; i++) {

        image1[i] = walls[i].image(emetteur);
        reflex1[i] = walls[i].refP(emetteur, recepteur);

        for (int j = 0; j < 3; j++) {
            if (j != i) {
                
                image2[i][j] = walls[j].image(image1[i]);
                reflex2[i][j][0] = walls[j].refP(recepteur, image1[i]);
                reflex2[i][j][1] = walls[i].refP(emetteur, reflex2[i][j][0]);

                for (int k = 0; k < 3; k++) {
                    if (k != j) {

                        image3[i][j][k] = walls[k].image(image2[i][j]);
                        reflex3[i][j][k][0] = walls[k].refP(recepteur, image2[i][j]);
                        reflex3[i][j][k][1] = walls[j].refP(image1[i], reflex3[i][j][k][0]);
                        reflex3[i][j][k][2] = walls[i].refP(reflex3[i][j][k][1], emetteur);

                    }
                }
            }
        }
    }
    double cos_angle1 = walls[1].cos_i(emetteur, recepteur);
    double cos_angle2 = walls[0].cos_i(image1[0], recepteur);
    double cos_angle3 = walls[1].cos_i(emetteur, reflex1[0]);
    cout << "Coeff trans 1" << walls[1].transmission(cos_angle1) << endl;
    cout << "Cos i = " << cos_angle2 << ", coeff reflex 2 = " << walls[1].reflection(cos_angle2) << endl;
    cout << "Cos i = " << cos_angle3 << ", coeff trans 2" << walls[1].transmission(cos_angle3) << endl;
    complex<double> coeff1[1] = { walls[1].transmission(cos_angle1) };
    complex<double> coeff2[2] = { walls[1].reflection(cos_angle2), walls[1].transmission(cos_angle3) };

    complex<double> champ1 = champ(coeff1, 1, (recepteur - emetteur).norm());
    complex<double> champ2 = champ(coeff2, 2, (recepteur - image1[0]).norm());
    cout << "E1 = " << champ1 << ", E2 = " << champ2 << endl;
    complex<double> champs[2] = { champ1, champ2 };
    cout << "P = " << power(champs, 2) << endl;

    if (!init()) {
        printf("Failed to initialize !\n");
    }
    else {
        bool quit = false;
        SDL_Event e;
        
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        line(recepteur, emetteur);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        point(recepteur);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        point(emetteur);
        
        /*for (int i = 0; i < *(&wallsPlane + 1) - wallsPlane; i++) {
            line(wallsPlane[i].X, wallsPlane[i].X2);
        }*/
        
        for (int i = 0; i < 3; i++) {

            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
            point(image1[i]);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            line(walls[i].X, walls[i].X2);

            if (walls[i].isReflected(recepteur, emetteur)) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                line(recepteur, reflex1[i]);
                line(emetteur, reflex1[i]);
            }
            for (int j = 0; j < 3; j++) {
                if (j != i) {
                    if (walls[j].isReflected(recepteur, reflex2[i][j][1]) && walls[i].isReflected(reflex2[i][j][0], emetteur)) {
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x88, 0x00, 0xFF);
                        point(image2[i][j]);
                        line(emetteur, reflex2[i][j][1]);
                        line(reflex2[i][j][1], reflex2[i][j][0]);
                        line(reflex2[i][j][0], recepteur);
                    }
                    

                    for (int k = 0; k < 3; k++) {
                        if (k != j) {
                            if (walls[k].isReflected(recepteur, reflex3[i][j][k][1]) && walls[j].isReflected(reflex3[i][j][k][0], reflex3[i][j][k][2]) && walls[i].isReflected(reflex3[i][j][k][1], emetteur)) {
                                cout << "i, j, k = " << i << ", " << j << ", " << k << endl;
                                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                                line(emetteur, reflex3[i][j][k][2]);
                                line(reflex3[i][j][k][2], reflex3[i][j][k][1]);
                                line(reflex3[i][j][k][1], reflex3[i][j][k][0]);
                                line(reflex3[i][j][k][0], recepteur);
                            }
                        }
                    }
                }
            }
            
        }
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