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
#define c 299792458
#define Ra 73

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;
complex<double> j(0, 1.0);            //nombre imaginaire j
double omega = 868300000 * 2 * M_PI;  //pulsation de l'onde
double lambda = 2 * M_PI * c / omega; //longueur d'onde
double beta = omega / c;              //nombre d'onde dans le vide
double eps = epsR * eps_0;            //epsilon
complex<double> Z_m = sqrt(mu_0 / (eps - j * sigma / omega)); //Z_m

double ptxgtx = 0.00164;

//Calculs pour déterminer gamma_m
double whatever = sigma / (omega * eps);
double whatever2 = sqrt(1 + whatever * whatever);
double whatever3 = omega * sqrt(mu_0 * eps / 2);
double alpha_m = whatever3 * sqrt(whatever2 - 1);
double beta_m = whatever3 * sqrt(whatever2 + 1);
complex<double> gamma_m(alpha_m, beta_m);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

double reflexion(Object point1, Object point2, Wall mur) {

    double xi = point1.x;
    double yi = point1.y;
    double dx = point2.x - xi;
    double dy = point2.y - yi;
    double d = sqrt(dx * dx + dy * dy);
    double cos_theta_i = abs((dx * mur.n.x + dy * mur.n.y) / d);
    double sin_theta_i = sqrt(1 - cos_theta_i * cos_theta_i);
    double sin_theta_t = sqrt(1 / epsR) * sin_theta_i;
    double cos_theta_t = sqrt(1 - sin_theta_t * sin_theta_t);
    double s = thickness / cos_theta_t;


    complex<double> whatever4 = Z_m * cos_theta_i;
    complex<double> whatever5 = Z_0 * cos_theta_t;
    auto ref_perp = (whatever4 - whatever5) / (whatever4 + whatever5);
    auto ref_perp2 = ref_perp * ref_perp;
    complex<double> we = exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_theta_t * sin_theta_i);
    auto ref_m = ref_perp - (1.0 - ref_perp2) * (ref_perp * we) / (1.0 - ref_perp2 * we);
    auto trans_m = (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * we);
    auto E = trans_m * sqrt(60 * ptxgtx) * exp(-j * beta * d) / d;
    double sum = norm(E);

    double P = lambda * lambda / (8 * M_PI * M_PI * Ra) * sum;
    /*cout << "Cos theta i  : " << cos_theta_i << endl;
    cout << "sin_theta_i : " << sin_theta_i << endl;
    cout << "sin_theta_t: " << sin_theta_t << endl;
    cout << "Cos theta t : " << cos_theta_t << endl;

    cout << "sum : " << sum << endl;
    cout << "s : " << s << endl;
    cout << "d : " << d << endl;
    cout << "dy : " << dy << endl;
    cout << "dx : " << dx << endl;
    cout << "ref_perp : " << ref_perp << endl;
    cout << "ref_m : " << ref_m << endl;
    cout << "trans_m : " << trans_m << endl;
    cout << "E : " << E << endl;
    cout << "P : " << P << endl;*/
    return P;
}

/*auto transmission(complex<double> ref_perp) {
    auto ref_perp2 = ref_perp * ref_perp;
    return (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * we);
}

double champ(complex<double> coeffs[]) {
    complex<double> coeff(1.0, 0.0);
    for (int i = 0; i < *(&coeffs + 1) - coeffs; i++) {
        coeff *= coeffs[i];
    }
    return coeff * sqrt(60 * ptxgtx) * exp(-j * beta * d) / d;
}

double puissance(complex<double> Es []) {
    double sum = 0;
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * sum;
}*/

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

double scale = 3;

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

    Object image = wall1.image(emetteur);
    cout << "Mur 1 : " << endl;
    double r = reflexion(image, recepteur, wall1);
    cout << "Mur 2 : " << endl;
    reflexion(emetteur, recepteur, wall2);

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