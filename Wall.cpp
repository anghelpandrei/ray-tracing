#include "Wall.h"
#include <cmath>
#include <iostream>
#include <SDL_stdinc.h>
#include <complex>
#define PI 3.14159265
#define eps_0 0.000000000008854187
#define mu_0 0.000001256637
#define c 299792458

using namespace std;


double Wall::cos_i(Object p1, Object p2) {
    Object d = p2 - p1;
    return abs(d.dotP(n) / d.norm());
}

double Wall::sin_i(double cos_i) {
    return sqrt(1 - cos_i * cos_i);
}

double Wall::sin_t(double sin_i) {
    return sqrt(1 / epsR) * sin_i;
}

double Wall::cos_t(double sin_t) {
    return sqrt(1 - sin_t * sin_t);
}



complex<double> Wall::we(double cos_i) {
    double sin_i = sqrt(1 - cos_i * cos_i);
    double sin_t = sqrt(1 / epsR) * sin_i;
    double s = thickness / cos_t(sin_t);

    return exp(-2.0 * gamma_m * s) * exp(j * beta * 2.0 * s * sin_t * sin_i);
}

complex<double> Wall::transmission(complex<double> ref_perp, double cos_i) {
    auto ref_perp2 = ref_perp * ref_perp;
    double s = thickness / cos_t(sin_t(sin_i(cos_i)));
    return (1.0 - ref_perp2) * exp(-gamma_m * s) / (1.0 - ref_perp2 * we(cos_i));
}

bool Wall::sameSide(Object point1, Object point2) {
    if (n.dotP(image(point1) - point1) * n.dotP(point2 - image(point2)) >= 0) {
        return false;
    }
    return true;
}

Object Wall::image(Object point) {
    return point - (2 * (point - Object(x,y)).dotP(n) * n);
}

Object Wall::symmetryP(Object point) {
    double xp = point.x;
    double yp = point.y;
    double t = (a * xp - yp + b) / (ny - a*nx);
    return Object(xp + t * nx, yp + t * ny);
}

Object Wall::reflexP(Object point1, Object point2) {
    Object s = symmetryP(point1);
    double xi = 2 * s.x - point1.x;
    double yi = 2 * s.y - point1.y; //bons calculs mais y a moyen d'optimiser oui^^
    double xp = point2.x;
    double yp = point2.y;
    double dx = xp - xi;
    double dy = yp - yi;

    double t = (dy * (xi - s.x) - dx * (yi - s.y))/(ux*dy - uy*dx);
    
    return Object(s.x + t * ux, s.y + t * uy);
}

Wall::Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma) :
Object(x, y), length(length), angle(angle), thickness(thickness), epsR(epsR), sigma(sigma)
{
    x2 = x + length * cos(angle * PI/180);
    y2 = y + length * sin(angle * PI/180);
    
    nx = y2 - y;
    ny = x - x2;
    
    n = Object(nx, ny);
    n = n/n.norm();
    nx = n.x;
    ny = n.y;
    

    a = (y2 - y) / (x2 - x);
    b = y - a * x;
    
    ux = x2 - x;
    uy = y2 - y;
    
    u = Object(ux, uy);
    u = u / u.norm();
    ux = u.x;
    uy = u.y;

    complex<double> j(0.0, 1.0);
    double omega = 868300000 * 2 * M_PI;
    double beta = omega / c;

    Z_m = sqrt(mu_0 / (epsR * eps_0 - j * sigma / omega));
    double whatever = sigma / (omega * epsR * eps_0);
    double whatever2 = sqrt(1 + whatever * whatever);
    double whatever3 = omega * sqrt(mu_0 * epsR * eps_0 / 2);
    alpha_m = whatever3 * sqrt(whatever2 - 1);
    beta_m = whatever3 * sqrt(whatever2 + 1);
    gamma_m = complex<double>(alpha_m, beta_m);
}