#include <iostream>
#include <complex> 
#include "Transmitter.h"
#include "Receiver.h"
#include "Wall.h"
#include <SDL.h>
#include <stdio.h>
#define epsR 4.8
#define sigma 0.018
#define thickness 0.15
#define Z_0 377
#define mu_0 0.000001256637
#define eps_0 0.000000000008854187
//#define R 73

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;
complex<double> j(0, 1.0);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool init();

void close();

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        window = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
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

void line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + x1, SCREEN_HEIGHT / 2 - y1, SCREEN_WIDTH / 2 + x2, SCREEN_HEIGHT / 2 - y2);
    //SDL_RenderDrawLine(renderer, x1, - y1,  x2,  y2);
}

void point(int x, int y) {
    SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2 + x, SCREEN_HEIGHT / 2 - y);
    //SDL_RenderDrawPoint(renderer, x, - y);
}

double reflexion(Object point1, Object point2, Wall mur) {
    
    double xi = point1.getX();
    double yi = point1.getY();
    double dx = point2.getX() - xi;
    double dy = point2.getY() - yi;
    double d = sqrt(dx * dx + dy * dy);
    double cos_theta_i = dx / d;
    double sin_theta_i = sqrt(1 - cos_theta_i * cos_theta_i);
    double sin_theta_t = sqrt(1 / epsR) * sin_theta_i;
    double cos_theta_t = sqrt(1 - sin_theta_t * sin_theta_t);
    double s = thickness / cos_theta_t;
    double omega = 868300000*2*M_PI;
    double eps = epsR * eps_0;
    double whatever = sigma / (omega * eps);
    double whatever2 = sqrt(1 + whatever * whatever);
    double whatever3 = omega * sqrt(mu_0 * eps / 2);
    double alpha = whatever3 * sqrt(whatever2 - 1);
    double beta = whatever3 * sqrt(whatever2 + 1);
    cout << "Alpha : " << alpha << endl;
    cout << "Beta : " << beta << endl;
    //double beta = 39.9;
    //complex<double> Z_m (171.57, 6.65);
    complex<double> Z_m = sqrt(mu_0/(eps-j*sigma/omega));
    cout << "Z_m : " << Z_m << endl;
    complex<double> gamma_m(alpha, beta);
    
   
    complex<double> whatever4 = Z_m * cos_theta_i;
    complex<double> whatever5 = Z_0 * cos_theta_t;
    auto ref_perp = (whatever4 - whatever5) / (whatever4 + whatever5);
    auto ref_perp2 = ref_perp * ref_perp;
    complex<double> we = exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_theta_t * sin_theta_i);
    auto ref_m = ref_perp - (1.0 - ref_perp2)*(ref_perp * we)/(1.0 - ref_perp2 * we);
    auto trans_m = (1.0-ref_perp2)*exp(-gamma_m*s)/(1.0-ref_perp2*we);
    auto E = sqrt(60 * 2);
    cout << "Cos theta t : " << cos_theta_t << endl;
    cout << "sin_theta_t: " << sin_theta_t << endl;
    cout << "sin_theta_i : " << sin_theta_i << endl;
    cout << "Cos theta i  : " << cos_theta_i << endl;
    cout << "s : " << s << endl;
    cout << "d : " << d << endl;
    cout << "dy : " << dy << endl;
    cout << "dx : " << dx << endl;
    cout << "ref_perp : " << ref_perp << endl;
    cout << "ref_m : " << ref_m << endl;
    double P = 0;
    return P;
}

double dotP(Object vec1, Object vec2) {
    return vec1.getX() * vec2.getX() + vec1.getY() * vec2.getY();
}

int main(int argc, char* argv[]) {
    
    std::cout << "Hello, World!" << endl;
    Transmitter emetteur(32, 10);
    Receiver recepteur(47, 65);
    Wall wall1 = Wall(0, 0, 80, 90, thickness, epsR, sigma);
    Wall wall2 = Wall(0, 20, 80, 0, thickness, epsR, sigma);
    Wall wall3 = Wall(0, 80, 80, 0, thickness, epsR, sigma);
    Wall walls[] = {wall1, wall2, wall3};
    double xe = emetteur.getX();
    double ye = emetteur.getY();
    double xr = recepteur.getX();
    double yr = recepteur.getY();
    Object image = wall1.image(emetteur);
    cout << "Mur 1 : " << endl;
    double r = reflexion(image, recepteur, wall1);
    cout << "Mur 2 : " << endl;
    reflexion(emetteur, recepteur, wall2);
    cout << "Mur 2 : " << endl;

    for (int i = 0; i < 3; i++) {
        cout << "Mur " << i << " : x = " << walls[i].getX() << ", y = " << walls[i].getY() << ", n = (" << walls[i].getNx() << ", " << walls[i].getNy() << ")" << endl;
    }



    if (!init()) {
        printf("Failed to initialize !\n");
    }
    else {
        bool quit = false;
        SDL_Event e;
        
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            //SDL_RenderSetScale(renderer, 2, 2);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            /*for (int i = 0; i < 3; i++) {

                double x1 = walls[i].getX(), x2 = walls[i].getX2(), y1 = walls[i].getY(), y2 = walls[i].getY2();
                double xei = walls[i].image(emetteur).getX(), yei = walls[i].image(emetteur).getY();
                double xri = walls[i].image(recepteur).getX(), yri = walls[i].image(recepteur).getY();

                Object pReflex = walls[i].reflexP(emetteur, recepteur);
                double xR = pReflex.getX();
                double yR = pReflex.getY();

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                line(x1, y1, x2, y2);

                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
                point(xei, yei);

                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
                point(xri, yri);

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                line(xr, yr, xR, yR);
                line(xe, ye, xR, yR);
            }*/

            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
            line(recepteur.getX(), recepteur.getY(), emetteur.getX(), emetteur.getY());


            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            point(recepteur.getX(), recepteur.getY());

            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            point(emetteur.getX(), emetteur.getY());

            SDL_RenderPresent(renderer);

        }
    }
    close();
    return 0;
}