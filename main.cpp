#include <iostream>
#include <complex> 
#include <vector>
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
double x_length = 44;
double y_length = 8;
double side_length = 0.2;
int x_dots = round(x_length / side_length);
int y_dots = round(y_length / side_length);


void line(const Vector2& p1, const Vector2& p2) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * p1.x), RENDER_Y0 + round(scale * p1.y), RENDER_X0 + round(scale * p2.x), RENDER_Y0 + round(scale * p2.y));
}
void point(const Vector2& p) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * (p.x - 0.5)), RENDER_Y0 + round(scale * (p.y - 0.5)), RENDER_X0 + round(scale * (p.x + 0.5)), RENDER_Y0 + round(scale * (p.y + 0.5)));
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * (p.x - 0.5)), RENDER_Y0 + round(scale * (p.y + 0.5)), RENDER_X0 + round(scale * (p.x + 0.5)), RENDER_Y0 + round(scale * (p.y - 0.5)));
}
void area(const Vector2& p, double power) {
    power = power < 0 ? 0.0001 : power;
    double red = 0;
    double green = 0;
    double blue = 0;
    if (power > 0.75) {
        red = 255.0;
        green = 255.0;
        blue = 1020.0 * power - 765.0;
    }
    else if (power > 0.33) {
        red = 255.0;
        green = 606.0 * power - 201.0;
        blue = 0;
    }
    else {
        red = 255.0 * 3 * power;
        green = 0;
        blue = 0;
    }
    SDL_Rect r = { RENDER_X0 + round(scale * (p.x - side_length)), RENDER_Y0 + round(scale * (p.y - side_length)), round(2 * side_length * scale), round(2* side_length * scale) };
    SDL_SetRenderDrawColor(renderer, red, green, blue, 0xFF);
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
const double sqrtptxTP8 = sqrt(60 * 0.00164);
const double sqrtptxPlane = sqrt(60 * 0.164);
void field(complex<double>& field, const complex<double>& coeff, const double d, const bool isLocal) {
    complex<double> result = coeff * exp(-j * beta * d) / d;
    if (isLocal) {
        result = norm(result) * sqrtptxPlane;
    }
    else {
        result *= sqrtptxTP8;
    }
    //cout << "champ = " << result << endl;
    field += result;
}

void transmission(Wall *walls, const int i, const Vector2& p1, const Vector2& p2, complex<double>& coeff) {
    complex<double> result = walls[i].transmission(p1, p2);
    coeff *= result;
    //cout << "Coeff de transmission du mur " << i << " = " << result << endl;
}

void reflection(Wall *walls, const int i, Vector2& p1, Vector2& p2, complex<double>& coeff) {
    complex<double> result = walls[i].reflection(p1, p2);
    coeff *= result;
    //cout << "Coeff de reflexion du mur " << i << " = " << result << endl;
}


double power(const complex<double>& field, bool isLocal) {
    if (isLocal) {
        return lambda * lambda / (8 * M_PI * M_PI * Ra) * real(field);
    }
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * norm(field);
}
complex<double> computation(Wall *walls, int n_walls, Vector2& emetteur, Vector2& recepteur, bool isLocal) {
    complex<double> coeff(1.0, 0.0);
    for (int i = 0; i < n_walls; i++) {
        if (walls[i].inter(emetteur, recepteur)) {
            transmission(walls, i, emetteur, recepteur, coeff);
        }
    }
    complex<double> champ = 0;
    field(champ, coeff, (recepteur - emetteur).norm(), isLocal);
    //cout << "Puissance = " << power(champ, false) << endl;

    for (int i = 0; i < n_walls; i++) {
        Vector2 reflex1;
        Vector2 image1 = walls[i].image(emetteur);
        coeff = complex<double>(1.0, 0.0);
        if (walls[i].inter(image1, recepteur, reflex1)) {
            //cout << "Mur " << i << endl;
            reflection(walls, i, image1, recepteur, coeff);
            for (int j = 0; j < n_walls; j++) {
                if (i != j && walls[j].inter(reflex1, recepteur)) {
                    transmission(walls, j, reflex1, recepteur, coeff);
                }
            }
            for (int j = 0; j < n_walls; j++) {
                if (i != j && walls[j].inter(emetteur, reflex1)) {
                    transmission(walls, j, emetteur, reflex1, coeff);
                }
            }
            field(champ, coeff, (recepteur - image1).norm(), isLocal);
            //cout << "Puissance = " << power(champ, false) << endl;
        }
        for (int j = 0; j < n_walls; j++) {
            if (j != i) {
                
                Vector2 reflex20;
                Vector2 reflex21;
                coeff = complex<double>(1.0, 0.0);
                Vector2 image2 = walls[j].image(image1);
                if (walls[j].inter(image2, recepteur, reflex20) && walls[i].inter(image1, reflex20, reflex21)) {
                    //cout << "Mur" << i << ", " << j << endl;
                    reflection(walls, j, image2, recepteur, coeff);
                    reflection(walls, i, image1, reflex20, coeff);
                    for (int k = 0; k < n_walls; k++) {
                        if (k != j && walls[k].inter(reflex20, recepteur)) {
                            transmission(walls, k, reflex20, recepteur, coeff);
                        }
                    }
                    for (int k = 0; k < n_walls; k++) {
                        if (k != j && k != i && walls[k].inter(reflex21, reflex20)) {
                            transmission(walls, k, reflex21, reflex20, coeff);
                        }
                    }
                    for (int k = 0; k < n_walls; k++) {
                        if (k != i && walls[k].inter(emetteur, reflex21)) {
                            transmission(walls, k, emetteur, reflex21, coeff);
                        }
                    }
                    field(champ, coeff, (recepteur - image2).norm(), isLocal);
                    //cout << "Puissance = " << power(champ, false) << endl;
                }
                //for (int k = 0; k < n_walls; k++) {
                //    if (k != j) {
                //        coeff = complex<double>(1.0, 0.0);
                //        Vector2 image3 = walls[k].image(image2);
                //        Vector2 reflex30;
                //        Vector2 reflex31;
                //        Vector2 reflex32;
                //        if (walls[k].inter(image3, recepteur, reflex30) && walls[j].inter(image2, reflex30, reflex31) && walls[i].inter(image1, reflex31, reflex32)) {
                //            //cout << "Mur" << i << ", " << j << ", " << k << endl;
                //            reflection(walls, k, image3, recepteur, coeff);
                //            reflection(walls, j, image2, reflex30, coeff);
                //            reflection(walls, i, image1, reflex31, coeff);
                //            for (int l = 0; l < n_walls; l++) {
                //                if (k != l && walls[l].inter(reflex30, recepteur)) {
                //                    transmission(walls, l, reflex30, recepteur, coeff);
                //                }
                //            }
                //            for (int l = 0; l < n_walls; l++) {
                //                if (k != l && j != l && walls[l].inter(reflex31, reflex30)) {
                //                    transmission(walls, l, reflex31, reflex30, coeff);
                //                }
                //            }
                //            for (int l = 0; l < n_walls; l++) {
                //                if (j != l && i != l && walls[l].inter(reflex32, reflex31)) {
                //                    transmission(walls, l, reflex32, reflex31, coeff);
                //                }
                //            }
                //            for (int l = 0; l < n_walls; l++) {
                //                if (i != l && walls[l].inter(emetteur, reflex32)) {
                //                    transmission(walls, l, emetteur, reflex32, coeff);
                //                }
                //            }
                //            field(champ, coeff, (recepteur - image3).norm(), isLocal);
                //            //cout << "Puissance = " << power(champ, false) << endl;
                //        }
                //    }
                //}
            }
        }
    }
    return champ;
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

        double* powers = new double[x_dots * y_dots];
        Vector2* receivers = new Vector2[x_dots * y_dots];
        double powerReferenceMax = -53;
        double powerReferenceMin = -78;
        double powerActualMax = -100.0;
        double powerActualMin = -100.0;

        int ind = 0;
        for (int i = 0; i < x_dots; i++) {
            for (int j = 0; j < y_dots; j++) {
                receivers[ind].x = -12 + (i + 0.5) * side_length;
                receivers[ind].y =  10 + (j + 0.5) * side_length;
                complex<double> field1 = computation(wallsPlane, n_walls, emetteur1, receivers[ind], true);
                complex<double> field2 = computation(wallsPlane, n_walls, emetteur2, receivers[ind], true);
                powers[ind] = power(field1 + field2, true);
                powers[ind] = 10 * log10(powers[ind] /0.001);
                if (powers[ind] < powerActualMin) {
                    //powerActualMin = powers[ind];
                }
                if (powers[ind] > powerActualMax) {
                    powerActualMax = powers[ind];
                }
                //cout << powers[ind] << endl;
                ind++;
            }
        }
        ind = 0;
        Vector2 translation = Vector2(0, -10);
        for (int i = 0; i < x_dots; i++) {
            for (int j = 0; j < y_dots; j++) {
                area(receivers[ind] + translation, (powers[ind] - powerActualMin) / (powerActualMax - powerActualMin));
                if (powers[ind] > powerReferenceMax) {
                    area(receivers[ind], 1);
                } else if (powers[ind] > powerReferenceMin) {
                    area(receivers[ind], (powers[ind] - powerReferenceMin) / (powerReferenceMax - powerReferenceMin));
                }
                ind++;
            }
        }
        delete[] powers;
        delete[] receivers;

        for (int i = 0; i < *(&wallsPlane + 1) - wallsPlane; i++) {
            if (wallsPlane[i].sigma > 0.5) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
            }
            line(wallsPlane[i].X, wallsPlane[i].X2);
            line(wallsPlane[i].X + translation, wallsPlane[i].X2 + translation);
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        point(emetteur1);
        point(emetteur2);
        
        double powerTP8 = power(computation(walls, 3, emetteurTP8, recepteurTP8, false), false);
        cout << "Power TP 8 : " << powerTP8 << endl;
        cout << "Power TP 8 (en dBm) : " << 10 * log10(powerTP8 / 0.001) << endl;
        SDL_RenderPresent(renderer);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        std::cout << "Duration (s) : " << duration.count()/1000.0 << endl;
        std::cout << "Size of double = " << sizeof(double) << endl;
        std::cout << "Size of vector2 = " << sizeof(Vector2) << endl;
        std::cout << "Size of Wall = " << sizeof(Wall) << endl;
        std::cout << "Size of complex = " << sizeof(complex<double>) << endl;

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