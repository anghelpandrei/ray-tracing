#include "Wall.h"
#include <cmath>
#include <iostream>
#include <complex>
#include "Constants.h"
#include <chrono>

using namespace std;

complex<double> Wall::transmission(const Object& p1, const Object& p2) {
    Object d = p2 - p1;
    double cos_i = abs(d.dotP(n) / d.norm());
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;
    complex<double> tempExp = exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_t * sin_i);
    complex<double> tempZm = Z_m * cos_i;
    double tempZ0 = Z_0 * cos_t;
    complex<double> ref_perp = (tempZm - tempZ0) / (tempZm + tempZ0);
    complex<double> ref_perp2 = ref_perp * ref_perp;
    return (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * tempExp);
    
}

complex<double> Wall::reflection(const Object& p1, const Object& p2) {
    Object d = p2 - p1;
    double cos_i = abs(d.dotP(n) / d.norm());
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double cos_t = sqrt(1 - sin_t * sin_t);
    double s = thickness / cos_t;
    complex<double> tempExp = exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_t * sin_i);
    complex<double> tempZm = Z_m * cos_i;
    double tempZ0 = Z_0 * cos_t;
    complex<double> ref_perp = (tempZm - tempZ0) / (tempZm + tempZ0);
    complex<double> ref_perp2 = ref_perp * ref_perp;

    return ref_perp - (1.0 - ref_perp2) * (ref_perp * tempExp) / (1.0 - ref_perp2 * tempExp);
}

Object Wall::image(const Object& p) {
    return p - (2 * (p - X).dotP(n) * n);
}

bool Wall::inter(const Object& p1, const Object& p2, Object& interP) {
    Object d = p2 - p1;
    double crossP = u.x * d.y - u.y * d.x;
    if (crossP == 0 || n.dotP(image(p1) - p1) * n.dotP(image(p2) - p2) >= 0) {
        return false;
    }
    double t = (d.y * (p1.x - x) - d.x * (p1.y - y)) / crossP;
    if (t < 0 || t > length) {
        return false;
    }
    interP = X + t * u;
    return true;
}

bool Wall::inter2(const Object& p1, const Object& p2) {
    Object d = p2 - p1;
    double crossP = u.x * d.y - u.y * d.x;
    if (crossP == 0 || n.dotP(image(p1) - p1) * n.dotP(image(p2) - p2) >= 0) {
        return false;
    }
    double t = (d.y * (p1.x - x) - d.x * (p1.y - y)) / crossP;
    if (t < 0 || t > length) {
        return false;
    }
    return true;
}

Wall::Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma) :
Object(x, y), length(length), angle(angle), thickness(thickness), epsR(epsR), sigma(sigma)
{
    X = Object(x, y);
    x2 = x + length * cos(angle * M_PI/180.0); //calculs des coordonnées de l'autre bout du mur
    y2 = y + length * sin(angle * M_PI/180.0);
    X2 = Object(x2, y2);
    
    n = Object(y2 - y, x - x2);  //calcul du vecteur normal
    n = n/n.norm();
    
    u = X2 - X;        //calcul du vecteur directeur
    u = u / u.norm();

    Z_m = sqrt(mu_0 / (epsR * eps_0 - j * sigma / omega));
    double temp = sigma / (omega * epsR * eps_0);
    double temp2 = sqrt(1 + temp * temp);
    double temp3 = omega * sqrt(mu_0 * epsR * eps_0 / 2);
    alpha_m = temp3 * sqrt(temp2 - 1);
    beta_m = temp3 * sqrt(temp2 + 1);
    gamma_m = complex<double>(alpha_m, beta_m);
}