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


/*
On introduit les différents paramètres pour l'affichage de l'avion
*/
const int SCREEN_WIDTH = 1200; // Largeur de la fenêtre
const int SCREEN_HEIGHT = 900; // Hauteur de la fenêtre
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
double scale = 25.0; // Echelle de visualisation  
int RENDER_X1 = SCREEN_WIDTH / 2;
int RENDER_Y1 = SCREEN_HEIGHT / 2;
int RENDER_X0 = SCREEN_WIDTH / 3.3;
int RENDER_Y0 = SCREEN_HEIGHT / 3;
double x_length = 44;
double y_length = 8;
double side_length = 0.2;
int x_dots = round(x_length / side_length);
int y_dots = round(y_length / side_length);

double red(const double power) {
    if (power > 0.33) {
        return 255.0;
    }
    return 255.0 * 3 * power;
}

double green(const double power) {
    if (power > 0.75) {
        return 255.0;
    }
    else if (power > 0.33) {
        return 606.0 * power - 199.97;
    }
    return 0;
}

double blue(const double power) {
    if (power > 0.75) {
        return 1020.0 * power - 765.0;
    }
    return 0;
}

void line(const Vector2& p1, const Vector2& p2) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * p1.x), RENDER_Y0 + round(scale * p1.y), RENDER_X0 + round(scale * p2.x), RENDER_Y0 + round(scale * p2.y));
}

void lineFade(const Vector2& p1, const Vector2& p2) {
    for (int i = 0; i < 255; i++) {
        SDL_SetRenderDrawColor(renderer, red(i/255.0), green(i/255.0), blue(i/255.0), 0xFF);
        SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * p1.x) + i, RENDER_Y0 + round(scale * p1.y), RENDER_X0 + round(scale * p2.x) + i, RENDER_Y0 + round(scale * p2.y));
    }
    
}

void point(const Vector2& p) {
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * (p.x - 0.5)), RENDER_Y0 + round(scale * (p.y - 0.5)), RENDER_X0 + round(scale * (p.x + 0.5)), RENDER_Y0 + round(scale * (p.y + 0.5)));
    SDL_RenderDrawLine(renderer, RENDER_X0 + round(scale * (p.x - 0.5)), RENDER_Y0 + round(scale * (p.y + 0.5)), RENDER_X0 + round(scale * (p.x + 0.5)), RENDER_Y0 + round(scale * (p.y - 0.5)));
}

void area(const Vector2& p, double power) {
/*
Cette fonction permet définir un pannel de couleur selon la puissance perçue 
*/
    power = power < 0 ? 0.0001 : power;
    SDL_Rect r = { RENDER_X0 + round(scale * (p.x - side_length)), RENDER_Y0 + round(scale * (p.y - side_length)), round(2 * side_length * scale), round(2* side_length * scale) };
    SDL_SetRenderDrawColor(renderer, red(power), green(power), blue(power), 0xFF);
    SDL_RenderFillRect(renderer, &r);
}



bool init() {
/*
Cette fonction permet la création de la fenêtre via la librairie SDL  
*/
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

const double sqrtptxgtxTP8 = sqrt(60.0 * 0.001 * 1.64);
const double sqrtptxgtxPlane = sqrt(60.0 * 0.1 * 1.64);



void transmission(Wall *walls, const int i, const Vector2& p1, const Vector2& p2, complex<double>& coeff) {
/*
Cette fonction multiplie coeff par un coefficient de transmission. On peut également afficher le coefficient avant la multiplication
*/
    complex<double> result = walls[i].transmission(p1, p2);
    coeff *= result;
    //cout << "Coeff de transmission du mur " << i << " = " << result << endl;
}

void reflection(Wall *walls, const int i, const Vector2& p1, const Vector2& p2, complex<double>& coeff) {
/*
Cette fonction multiplie coeff par un coefficient de réflexion. On peut également afficher le coefficient avant la multiplication
*/
    complex<double> result = walls[i].reflection(p1, p2);
    coeff *= result;
    //cout << "Coeff de reflexion du mur " << i << " = " << result << endl;
}


double power(const complex<double>& field, bool isLocal) {
/*
Cette fonction permet de calculer la puissance perçue dans chaque zone locale  
*/
    if (isLocal) {
        return lambda * lambda / (8 * M_PI * M_PI * Ra) * field.real();
    }
    return lambda * lambda / (8 * M_PI * M_PI * Ra) * norm(field);
}

void field(complex<double>& field, const complex<double>& coeff, const double d, const bool isLocal) {
    /*
    Cette fonction permet de calculer la valeur du champ électrique selon si l'on considère le cas de l'avion ou le TP8
    */
    complex<double> result = coeff * exp(-j * beta * d) / d;
    if (isLocal) {
        result = norm(result * sqrtptxgtxPlane);
    }
    else {
        result *= sqrtptxgtxTP8;
        cout << "Puissance = " << power(result, false) << " W = " << 10*log10(power(result, false)/0.001) << " dBm" << endl;
    }
    //cout << "champ = " << result << endl;
    field += result;
}

//complex<double> recursive(Wall* walls, int n_walls, Vector2& emetteur,
//    Vector2& recepteur, bool isLocal, complex<double>& champ, int n, int n_max) {
//    n++;
//    for (int i = 0; i < n_walls; i++) {
//        Vector2 reflex1;
//        Vector2 image1 = walls[i].image(emetteur);
//        complex<double> coeff(1.0, 0.0);
//        if (walls[i].inter(image1, recepteur, reflex1)) {
//            //cout << "Mur " << i << endl;
//            reflection(walls, i, image1, recepteur, coeff);
//            for (int j = 0; j < n_walls; j++) {
//                if (i != j && walls[j].inter(reflex1, recepteur)) {
//                    transmission(walls, j, reflex1, recepteur, coeff);
//                }
//            }
//            for (int j = 0; j < n_walls; j++) {
//                if (i != j && walls[j].inter(emetteur, reflex1)) {
//                    transmission(walls, j, emetteur, reflex1, coeff);
//                }
//            }
//            field(champ, coeff, (recepteur - image1).norm(), isLocal);
//        }
//        if (n < n_max) {
//            recursive(walls, n_walls, emetteur, recepteur, isLocal, champ, n, n_max);
//        }
//        
//    }
//}

complex<double> computation(Wall *walls, int n_walls, Vector2& emetteur, Vector2& recepteur, bool isLocal) {
/*
Cette fonction permet de calculer le champ électrique pour un émetteur et un récepteur 
*/
    complex<double> coeff(1.0, 0.0); //produit des coefficients de réflexion et de transmission
    /*
    * On calcule en premier lieu le champ électrique de l'onde directe.
    * Pour ce faire, on vérifie quels murs cette onde intersecte et on multiplie coeff par le coefficient de tranmission correspondant
    */
    for (int i = 0; i < n_walls; i++) {
        if (walls[i].inter(emetteur, recepteur)) {
            transmission(walls, i, emetteur, recepteur, coeff);
        }
    }
    complex<double> champ = 0; //champ électrique qui va être return à la fin de la fonction computation
    field(champ, coeff, (recepteur - emetteur).norm(), isLocal);
    //cout << "Puissance = " << power(champ, false) << endl;

    for (int i = 0; i < n_walls; i++) {
        /* 
        * Boucle pour une réflexion :
        * Pour chaque mur, on calcule l'image de l'émetteur par celui-ci
        * Ensuite on vérifie par quels murs il y a réflexion (en vérifiant s'il y a des intersections entre le mur et le vecteur image-récepteur)
        * S'il y a réflexion, on calcule le coefficient de réflexion par le mur.
        * Dans le cas de réflexion, on calcule aussi les différentes intersections possibles entre les murs et les différentes composantes de l'onde
        * (dans ce cas, on regarde si l'onde partant de l'émetteur intersecte des murs jusqu'à arriver au point de réflexion, et aussi s'il y a des intersections
        * en partant du point de réflexion vers le récepteur)
        */
        Vector2 reflex1;
        Vector2 image1 = walls[i].image(emetteur);
        coeff = complex<double>(1.0, 0.0);
        if (walls[i].inter(image1, recepteur, reflex1)) { //Lorsqu'elle a 3 arguments, la fonction inter fixe la valeur du point d'intersection dans le 3ème argument
            //cout << "Mur " << i << endl;
            reflection(walls, i, image1, recepteur, coeff);
            for (int j = 0; j < n_walls; j++) {
                /*
                * i != j permet de vérifier qu'on ne calcule pas accidentellement le coefficient de transmission
                * de l'onde avec le mur où il y a la réflexion
                */
                if (i != j && walls[j].inter(reflex1, recepteur)) { 
                    transmission(walls, j, reflex1, recepteur, coeff);
                }
            }
            for (int j = 0; j < n_walls; j++) {
                if (i != j && walls[j].inter(emetteur, reflex1)) {
                    transmission(walls, j, emetteur, reflex1, coeff);
                }
            }
            field(champ, coeff, (recepteur - image1).norm(), isLocal); //la distance parcourue par l'onde est la norme entre le récepteur et l'image
            //cout << "Puissance = " << power(champ, false) << endl;
        }
        for (int j = 0; j < n_walls; j++) {
            /* Boucle pour deux réflexions, similaire au cas à une réflexion mais cette fois on a 2 points de réflexions, 2 images et 3 composantes du trajet où il y aurait des intersections */
            if (j != i) {
                Vector2 reflex20;
                Vector2 reflex21;
                coeff = complex<double>(1.0, 0.0);
                Vector2 image2 = walls[j].image(image1);
                if (walls[j].inter(image2, recepteur, reflex20) && walls[i].inter(image1, reflex20, reflex21)) { //ici on vérifie qu'il y a bien les 2 réflexions qui sont possibles
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
                // /* Boucle pour 3 réflexions, même chose mais avec 3 points de réflexion, 3 images et 4 composantes de trajet */
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
        Vector2 translation = Vector2(0, -10);
        double* powers = new double[x_dots * y_dots];
        Vector2* receivers = new Vector2[x_dots * y_dots];
        int powerReferenceMax = -53.0;  //puissance au-dessus de laquelle le débit est maximal
        int powerReferenceMin = -78.0;  //puissance en-dessous de laquelle le débit est minimal
        double powerActualMax = -100.0; //puissance maximale
        double powerActualMin = -130.0;    //puissance minimale

        int ind = 0;
        for (int i = 0; i < x_dots; i++) {
            cout << "Recepteur " << i + 1 << "/" << x_dots << endl;
            for (int j = 0; j < y_dots; j++) {
                /*
                * Calcul des coordonnées du récepteur, on commence à(-12, 10) (décalé un peu pour avoir les coins du carré(-12, 10))
                * et on ajoute une longeur de côté de zone à chaque itération de manière à avoir x_dots zones horizontalement et y_dots zones verticalement
                */
                
                receivers[ind].x = -12 + (i + 0.5) * side_length; 
                receivers[ind].y =  10 + (j + 0.5) * side_length;
                complex<double> field1 = computation(wallsPlane, n_walls, emetteur1, receivers[ind], true); //calcul du champ électrique pour le premier émetteur
                complex<double> field2 = computation(wallsPlane, n_walls, emetteur2, receivers[ind], true); //calcul du champ électrique pour le 2ème émetteur
                powers[ind] = power(field1 + field2, true);   //calcul de la puissance après avoir 
                powers[ind] = 10 * log10(powers[ind] /0.001); //conversion en décibels
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
        cout << "powerActualMin = " << powerActualMin << endl;
        cout << "powerActualMax = " << powerActualMax << endl;
        ind = 0;
        
        for (int i = 0; i < x_dots; i++) {
            for (int j = 0; j < y_dots; j++) {
                /* 
                * Pour l'affichage, on doit mettre les valeurs de puissance sur une échelle de 0 à 1
                * Pour l'affichage de la puissance de l'avion, on va de la valeur minimale de la puissance (0) à la valeur maximale de la puissance (1)
                * Pour l'affichage du débit, on va de -78 dB (0) à -53 dB (1)
                */
                area(receivers[ind] + translation, (powers[ind] - powerActualMin) / (powerActualMax - powerActualMin)); //affichage de la puissance dans l'avion
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

        lineFade(wallsPlane[0].X + Vector2(0, 10), wallsPlane[0].X + Vector2(0, 11));
        lineFade(wallsPlane[0].X - Vector2(0, 2) + translation, wallsPlane[0].X - Vector2(0, 3) + translation);
        complex<double> champTP8 = computation(walls, 3, emetteurTP8, recepteurTP8, false);
        cout << "Champ TP 8 : " << champTP8 << endl;
        double powerTP8 = power(champTP8, false);
        cout << "Power TP 8 : " << powerTP8 << endl;
        cout << "Power TP 8 (en dBm) : " << 10 * log10(powerTP8 / 0.001) << endl;
        SDL_RenderPresent(renderer);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        std::cout << "Duration (s) : " << duration.count()/1000.0 << endl;
        
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