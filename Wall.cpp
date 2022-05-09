#include "Wall.h"
#include <cmath>
#include <iostream>
#include <SDL_stdinc.h>
#include <complex>
#include "Constants.h"

using namespace std;


double Wall::cos_i(Object p1, Object p2) {
    Object d = p2 - p1;

    return abs(d.dotP(n) / d.norm());
}

complex<double> Wall::ref_perp(double cos_i) {
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    complex<double> temp = Z_m * cos_i;
    complex<double> temp2 = Z_0 * cos_t;

    return (temp - temp2) / (temp + temp2);
}

complex<double> Wall::we(double cos_i) {
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;

    return exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_t * sin_i);
}

complex<double> Wall::transmission(double cos_i) {
    complex<double> ref_perp = Wall::ref_perp(cos_i);
    complex<double> ref_perp2 = ref_perp * ref_perp;
    complex<double> we = Wall::we(cos_i);
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;

    return (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * we);
}

complex<double> Wall::reflection(double cos_i) {
    complex<double> ref_perp = Wall::ref_perp(cos_i);
    complex<double> ref_perp2 = ref_perp * ref_perp;
    complex<double> we = Wall::we(cos_i);
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;

    return ref_perp - (1.0 - ref_perp2) * (ref_perp * we) / (1.0 - ref_perp2 * we);
}

bool Wall::isReflected(Object p1, Object p2) {
    if (n.dotP(image(p1) - p1) * n.dotP(p2 - image(p2)) >= 0) {
        return false;
    }
    return true;
}

Object Wall::image(Object p) {
    return p - (2 * (p - X).dotP(n) * n);
}

Object Wall::interP(Object p1, Object p2) {
    Object d = p2 - p1;
    double t = (d.y * (p1.x - x) - d.x * (p1.y - y)) / (u.x * d.y - u.y * d.x);

    return Object(x + t * u.x, y + t * u.y);
}


Object Wall::refP(Object p1, Object p2) {
    p1 = image(p1);
    Object d = p2 - p1;
    double t = (d.y * (p1.x - x) - d.x * (p1.y - y)) / (u.x * d.y - u.y * d.x);

    return Object(x + t * u.x, y + t * u.y);
}

Wall::Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma) :
Object(x, y), length(length), angle(angle), thickness(thickness), epsR(epsR), sigma(sigma)
{
    X = Object(x, y);
    x2 = x + length * cos(angle * M_PI/180); //calculs des coordonnées de l'autre bout du mur
    y2 = y + length * sin(angle * M_PI/180);
    X2 = Object(x2, y2);
    
    n = X.crossP(X2);  //calcul du vecteur normal
    n = n/n.norm();
    
    u = X2 - X;        //calcul du vecteur directeur
    u = u / u.norm();  

    

    Z_m = sqrt(mu_0 / (epsR * eps_0 - j * sigma / omega));
    double whatever = sigma / (omega * epsR * eps_0);
    double whatever2 = sqrt(1 + whatever * whatever);
    double whatever3 = omega * sqrt(mu_0 * epsR * eps_0 / 2);
    alpha_m = whatever3 * sqrt(whatever2 - 1);
    beta_m = whatever3 * sqrt(whatever2 + 1);
    gamma_m = complex<double>(alpha_m, beta_m);
}