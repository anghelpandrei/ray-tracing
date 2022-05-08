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

double scale = 3;

void line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * x1, SCREEN_HEIGHT / 2 - scale * y1, SCREEN_WIDTH / 2 + scale * x2, SCREEN_HEIGHT / 2 - scale * y2);
    //SDL_RenderDrawLine(renderer, x1, - y1,  x2,  y2);
}

void point(int x, int y) {
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * x - 2, SCREEN_HEIGHT / 2 - scale * y - 2, SCREEN_WIDTH / 2 + scale * x + 2, SCREEN_HEIGHT / 2 - scale * y + 2);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2 + scale * x - 2, SCREEN_HEIGHT / 2 - scale * y + 2, SCREEN_WIDTH / 2 + scale * x + 2, SCREEN_HEIGHT / 2 - scale * y - 2);
    //SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2 + scale * x, SCREEN_HEIGHT / 2 - scale * y);
}

double reflexion(Object point1, Object point2, Wall mur) {
    
    double xi = point1.x;
    double yi = point1.y;
    double dx = point2.x - xi;
    double dy = point2.y - yi;
    double d = sqrt(dx * dx + dy * dy);
    double cos_theta_i = abs((dx * mur.nx + dy * mur.ny) / d);
    double sin_theta_i = sqrt(1 - cos_theta_i * cos_theta_i);
    double sin_theta_t = sqrt(1 / epsR) * sin_theta_i;
    double cos_theta_t = sqrt(1 - sin_theta_t * sin_theta_t);
    double s = thickness / cos_theta_t;


    complex<double> whatever4 = Z_m * cos_theta_i;
    complex<double> whatever5 = Z_0 * cos_theta_t;
    auto ref_perp = (whatever4 - whatever5) / (whatever4 + whatever5);
    auto ref_perp2 = ref_perp * ref_perp;
    complex<double> we = exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_theta_t * sin_theta_i);
    auto ref_m = ref_perp - (1.0 - ref_perp2)*(ref_perp * we)/(1.0 - ref_perp2 * we);
    auto trans_m = (1.0-ref_perp2)*exp(-gamma_m*s)/(1.0-ref_perp2*we);
    auto E = trans_m * sqrt(60 * ptxgtx) * exp(-j*beta*d)/d;
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



int main(int argc, char* argv[]) {
    
    cout << "Hello, World!" << endl;
    Transmitter emetteur(32, 10);
    Receiver recepteur(47, 65);
    Wall wall1 = Wall(0, 0, 80, 90, thickness, epsR, sigma);
    Wall wall2 = Wall(0, 20, 80, 0, thickness, epsR, sigma);
    Wall wall3 = Wall(0, 80, 80, 0, thickness, epsR, sigma);
    Wall walls[] = {wall1, wall2, wall3};
    double xe = emetteur.x;
    double ye = emetteur.y;
    double xr = recepteur.x;
    double yr = recepteur.y;
    Object image = wall1.image(emetteur);
    cout << "Mur 1 : " << endl;
    double r = reflexion(image, recepteur, wall1);
    cout << "Mur 2 : " << endl;
    reflexion(emetteur, recepteur, wall2);

    /*for (int i = 0; i < 3; i++) {
        cout << "Mur " << i << " : x = " << walls[i].getX() << ", y = " << walls[i].getY() << ", n = (" << walls[i].getNx() << ", " << walls[i].getNy() << ")" << endl;
    }*/

    double x1murs[3];
    double y1murs[3];
    double x2murs[3];
    double y2murs[3];
    double xei[3];
    double yei[3];
    Object pReflex[3];
    double xReflex[3];
    double yReflex[3];

    double xImage1[3];
    double yImage1[3];
    double xImage2[3][3];
    double yImage2[3][3];

    double xReflex1[3];
    double yReflex1[3];
    double xReflex2[3];
    double yReflex2[3];


    double xImage3[3][3][3];
    double yImage3[3][3][3];

    double xReflex31[3][3][3];
    double yReflex31[3][3][3];
    double xReflex32[3][3][3];
    double yReflex32[3][3][3];
    double xReflex33[3][3][3];
    double yReflex33[3][3][3];


    bool trueReflex[3] = { true, true, true };
    bool trueReflex2[3][3] = { {true, true, true}, {true, true, true}, {true, true, true} };
    bool trueReflex3[3][3][3] = { { {true, true, true}, {true, true, true}, {true, true, true} }, { {true, true, true}, {true, true, true}, {true, true, true} }, { {true, true, true}, {true, true, true}, {true, true, true} } };

    for (int i = 0; i < 3; i++) {

        x1murs[i] = walls[i].x;
        y1murs[i] = walls[i].y;
        x2murs[i] = walls[i].x2;
        y2murs[i] = walls[i].y2;
        
        Object image1 = walls[i].image(emetteur);

        xei[i] = image1.x;
        yei[i] = image1.y;

        pReflex[i] = walls[i].reflexP(emetteur, recepteur);
        xReflex[i] = pReflex[i].x;
        yReflex[i] = pReflex[i].y;

        

        for (int j = 0; j < 3; j++) {
            if (j != i) {
                
                Object image2 = walls[j].image(image1);
                
                Object reflex1 = walls[j].reflexP(recepteur, image1);
                Object reflex2 = walls[i].reflexP(emetteur, reflex1);

                /*if (dotP(Object(image1.getX() - xe, image1.getY() - ye), walls[i].getN()) * dotP(Object(xr - walls[i].symmetryP(recepteur).getX(), yr - walls[i].symmetryP(recepteur).getY()), walls[i].getN()) >= 0) {
                    trueReflex[i] = false;
                }*/

                cout << "reflex1.getX = " << reflex1.x << endl;
                cout << "reflex1.getY = " << reflex1.x << endl;

                cout << "reflex2.getX = " << reflex2.x << endl;
                cout << "reflex2.getY = " << reflex2.y << endl;

                cout << "recepteur X = " << recepteur.x << endl;
                cout << "recepteur Y = " << recepteur.y << endl;

                cout << "image 1 X = " << image1.x << endl;
                cout << "image 1 Y = " << image1.y << endl;

                xImage1[i] = image1.x;
                yImage1[i] = image1.y;
                xImage2[i][j] = image2.x;
                yImage2[i][j] = image2.y;

                xReflex1[i] = reflex1.x;
                yReflex1[i] = reflex1.y;
                xReflex2[i] = reflex2.x;
                yReflex2[i] = reflex2.y;
                cout << "xReflex = " << xReflex1[i] << endl;
                for (int k = 0; k < 3; k++) {
                    if (k != j) {
                        cout << "i, j, k = " << i << ", " << j << ", " << k << endl;
                        Object image3 = walls[k].image(image2);
                        Object reflex31 = walls[k].reflexP(recepteur, image2);
                        Object reflex32 = walls[j].reflexP(reflex31, image1);
                        Object reflex33 = walls[i].reflexP(reflex32, emetteur);



                        xImage3[i][j][k] = image3.x; //+ de 3 images
                        yImage3[i][j][k] = image3.y;

                        //cout << "xImage3 = " << xImage3[i][j][k] << endl;
                        //cout << "yImage3 = " << yImage3[i][j][k] << endl;

                        xReflex31[i][j][k] = reflex31.x;
                        yReflex31[i][j][k] = reflex31.y;
                        xReflex32[i][j][k] = reflex32.x;
                        yReflex32[i][j][k] = reflex32.y;
                        xReflex33[i][j][k] = reflex33.x;
                        yReflex33[i][j][k] = reflex33.y;

                        cout << "xReflex31 = " << xReflex31[i][j][k] << endl;
                        cout << "yReflex31 = " << yReflex31[i][j][k] << endl;
                        cout << "xReflex32 = " << xReflex32[i][j][k] << endl;
                        cout << "yReflex32 = " << yReflex32[i][j][k] << endl;
                        cout << "xReflex33 = " << xReflex33[i][j][k] << endl;
                        cout << "yReflex33 = " << yReflex33[i][j][k] << endl;

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
        
        for (int i = 0; i < 3; i++) {

            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
            point(xei[i], yei[i]);

            

            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            line(x1murs[i], y1murs[i], x2murs[i], y2murs[i]);

            if (trueReflex[i]) { //à changer
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                line(xr, yr, xReflex[i], yReflex[i]);
                line(xe, ye, xReflex[i], yReflex[i]);
                for (int j = 0; j < 3; j++) {
                    if (j != i) {
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x88, 0x00, 0xFF);
                        point(xImage1[i], yImage1[i]);
                        point(xImage2[i][j], yImage2[i][j]);
                        line(xe, ye, xReflex2[i], yReflex2[i]);
                        line(xr, yr, xReflex1[i], yReflex1[i]);
                        line(xReflex1[i], yReflex1[i], xReflex2[i], yReflex2[i]);
                        
                        /*for (int k = 0; k < 3; k++) {
                            if (k != j && i == 2) {
                                cout << "i, j, k = " << i << ", " << j << ", " << k << endl;
                                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                                line(xe, ye, xReflex33[i][j][k], yReflex33[i][j][k]);
                                line(xReflex32[i][j][k], yReflex32[i][j][k], xReflex33[i][j][k], yReflex33[i][j][k]);
                                line(xReflex32[i][j][k], yReflex32[i][j][k], xReflex31[i][j][k], yReflex31[i][j][k]);
                                line(xr, yr, xReflex31[i][j][k], yReflex31[i][j][k]);
                            }
                        }*/
                    }
                    
                }
            }

            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
            point(xei[i], yei[i]);
            
        }



        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        line(xr, yr, xe, ye);


        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        point(xr, yr);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        point(xe, ye);

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