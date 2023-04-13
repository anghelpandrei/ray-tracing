#include "Wall.h"
#include <cmath>
#include <iostream>
#include <complex>
#include "Constants.h"
#include <chrono>

using namespace std;

complex<double> Wall::transmission(const Vector2& p1, const Vector2& p2) {
/*
Cette fonction permet de calculer la valeur du coefficient de transmission
*/
    /* Le vecteur d est le vecteur qui va du point 1 au point 2 */
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    double cos_i = abs(((dx * nx) + (dy * ny)) / sqrt(dx * dx + dy * dy));
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;
    complex<double> tempExp = exp(-2.0 * gamma_m * s) 
        * exp(j * beta * 2.0 * s * sin_t * sin_i);
    complex<double> tempZm = Z_m * cos_i;
    double tempZ0 = Z_0 * cos_t;
    complex<double> ref_perp = (tempZm - tempZ0) / (tempZm + tempZ0);
    complex<double> ref_perp2 = ref_perp * ref_perp;
    /*
    Ci-dessous, on imprime les différentes valeurs pour la résolution du TP 8
    */
    /*
    cout << "cos_theta_i = " << cos_i << endl;
    cout << "sin_theta_i = " << sin_i << endl;
    cout << "sin_theta_t = " << sin_t << endl;
    cout << "cos_theta_t = " << cos_t << endl;
    cout << "s = " << s << endl;
    cout << "ref_perp = " << ref_perp << endl;
    */
    return (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * tempExp);
    
}

complex<double> Wall::reflection(const Vector2& p1, const Vector2& p2) {
/*
Cette fonction permet de calculer la valeur du coefficient de réflexion 
*/
    /* Le vecteur d est le vecteur qui va du point 1 au point 2 */
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    double cos_i = abs(((dx * nx) + (dy * ny)) / sqrt(dx * dx + dy * dy)); 
    double sin_i = sqrt(1 - cos_i * cos_i);  
    double sin_t = sqrt(1 / epsR) * sin_i;    
    double cos_t = sqrt(1 - sin_t * sin_t);     
    double s = thickness / cos_t;
    complex<double> tempExp = exp(-2.0 * gamma_m * s) 
        * exp(j * beta * 2.0 * s * sin_t * sin_i);
    complex<double> tempZm = Z_m * cos_i;
    double tempZ0 = Z_0 * cos_t;
    complex<double> ref_perp = (tempZm - tempZ0) / (tempZm + tempZ0);  //calcul du coefficient de réflexion pour une polarisation perpendiculaire 
    
    complex<double> ref_perp2 = ref_perp * ref_perp;
    /*
    Ci-dessous, on imprime les différentes valeurs pour la résolution du TP 8
    */
    /*
    * cout << "cos_theta_i = " << cos_i << endl;
    * cout << "sin_theta_i = " << sin_i << endl;
    * cout << "sin_theta_t = " << sin_t << endl;
    * cout << "cos_theta_t = " << cos_t << endl;
    * cout << "s = " << s << endl;
    * cout << "ref_perp = " << ref_perp << endl;
    */
    return ref_perp - (1.0 - ref_perp2) * (ref_perp * tempExp) / (1.0 - ref_perp2 * tempExp);

}

Vector2 Wall::image(const Vector2& p) {
    /*
    * Pour calculer l'image, on part du point et on ajoute 2 fois un vecteur (partant du point jusqu'au mur orthogonalement au mur)
    * Pour ce faire on calcul le produit scalaire entre le vecteur qui va d'un point du mur jusqu'au point qui nous intéresse et le vecteur normal au mur
    * Ensuite on soustrait 2 fois ce produit scalaire multiplié par le vecteur normal aux coordonnées du point initial
    */
    double dotP = 2 * ((p.x - x) * nx + (p.y - y) * ny);
    return Vector2(p.x - dotP * nx, p.y - dotP * ny);
}

bool Wall::inter(const Vector2& p1, const Vector2& p2, Vector2& interP) {
/*
Fonction qui permet de voir s'il y a une inteection entre le vecteur qui relie le point p1 et le point p2 et le mur    
*/

    double dx = p2.x - p1.x; //composantes du vecteur d
    double dy = p2.y - p1.y;
    double crossP = ux * dy - uy * dx; //norme du produit vectoriel (en 3D) du vecteur directeur par le vecteur d

    if (abs(crossP) < epsilon) { //le vecteur directeur et le vecteur d sont colineaires, il n'y a pas d'intersection
        return false;
    }

    double p1x = p1.x - x;
    double p1y = p1.y - y;

    double p2x = p2.x - x;
    double p2y = p2.y - y;

    double dot1 = nx * p1x + ny * p1y;
    double dot2 = nx * p2x + ny * p2y;

    if (dot1 * dot2 >= epsilon) { //Vérification que les 2 points sont du même côté du mur
        return false;
    }
    double t = (dy * p1x - dx * p1y) / crossP;
    if (t < -epsilon || t > length + epsilon) { // Vérification que le point se trouve sur le mur et pas seulement sur la droite prolongeant le mur
        return false;
    }
    interP.x = x + t * ux;
    interP.y = y + t * uy;
    return true;
}

bool Wall::inter(const Vector2& p1, const Vector2& p2) { 
    // Même fonction que l'autre fonction d'intersection mais sans donner de valeur pour le point d'intersection

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double crossP = ux * dy - uy * dx;

    if (abs(crossP) < epsilon) {
        return false;
    }

    double p1x = p1.x - x;
    double p1y = p1.y - y;

    double p2x = p2.x - x;
    double p2y = p2.y - y;

    double dot1 = nx * p1x + ny * p1y;
    double dot2 = nx * p2x + ny * p2y;

    if (dot1 * dot2 >= epsilon) {
        return false;
    }

    double t = (dy * p1x - dx * p1y) / crossP;
    if (t < -epsilon || t > length + epsilon) {
        return false;
    }
    return true;
}

Wall::Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma) :
Vector2(x, y), length(length), angle(angle), thickness(thickness), epsR(epsR), sigma(sigma)
{
/*
Quand on initialise un mur, on calcule les différentes valeurs utiles telles que gamma_m, Z_m, le vecteur normal et directeur
*/
    X = Vector2(x, y);
    x2 = x + length * cos(angle * M_PI/180.0); //calculs des coordonnées de l'autre bout du mur
    y2 = y + length * sin(angle * M_PI/180.0);
    X2 = Vector2(x2, y2);
    
    nx = (y2 - y) / length;                               //calcul du vecteur normal
    ny = (x - x2) / length;
    
    ux = (x2 - x) / length;                               //calcul du vecteur directeur
    uy = (y2 - y) / length;

    Z_m = sqrt(mu_0 / (epsR * eps_0 - j * sigma / omega)); //impédance du mur
    double temp = sigma / (omega * epsR * eps_0);          //variables temporaires pour le calcul de alpha_m, beta_m et gamma_m
    double temp2 = sqrt(1 + temp * temp);
    double temp3 = omega * sqrt(mu_0 * epsR * eps_0 / 2);
    double alpha_m = temp3 * sqrt(temp2 - 1);
    double beta_m = temp3 * sqrt(temp2 + 1);
    gamma_m = complex<double>(alpha_m, beta_m);
}