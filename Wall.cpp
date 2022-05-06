#include "Wall.h"
#include <cmath>
#include <iostream>
#define PI 3.14159265


double Wall::getLength() const {
    return length;
}

void Wall::setLength(double length) {
    Wall::length = length;
}

double Wall::getAngle() const {
    return angle;
}

void Wall::setAngle(double angle) {
    Wall::angle = angle;
}

double Wall::getThickness() const {
    return thickness;
}

void Wall::setThickness(double thickness) {
    Wall::thickness = thickness;
}

double Wall::getEpsR() const {
    return epsR;
}

void Wall::setEpsR(double epsR) {
    Wall::epsR = epsR;
}

double Wall::getSigma() const {
    return sigma;
}

void Wall::setSigma(double sigma) {
    Wall::sigma = sigma;
}

double Wall::getX2() const {
    return x2;
}

void Wall::setX2(double x2) {
    Wall::x2 = x2;
}

double Wall::getY2() const {
    return y2;
}

void Wall::setY2(double y2) {
    Wall::y2 = y2;
}

double Wall::getNx() const {
    return nx;
}

void Wall::setNx(double nx) {
    Wall::nx = nx;
}

double Wall::getNy() const {
    return ny;
}

void Wall::setNy(double ny) {
    Wall::ny = ny;
}

double Wall::getUx() const {
    return ux;
}

double Wall::getUy() const {
    return uy;
}




Object Wall::symmetryP(Object point) {
    double xp = point.getX();
    double yp = point.getY();
    double t = (a * xp - yp + b) / (ny - a*nx);
    return Object(xp + t * nx, yp + t * ny);
}

/*Object Wall::image(Object point) {
    double xp = point.getX();
    double yp = point.getY();
    double dotP = (xp - x) * nx + (yp - y) * ny; //gros problème ici, je ne sais pas le résoudre mais pas d'impact pr le moment
    //std::cout << "xp = " << xp << ", yp = " << yp << ", dotP = " << dotP << std::endl;
    return Object(xp - (2 * nx * dotP), yp - (2 * ny * dotP));
}*/
Object Wall::image(Object point) {
    Object s = symmetryP(point);
    return Object(2 * s.getX() - point.getX(), 2 * s.getY() - point.getY());
}

Object Wall::reflexP(Object point1, Object point2) {
    Object s = symmetryP(point1);
    double xi = 2 * s.getX() - point1.getX();
    double yi = 2 * s.getY() - point1.getY(); //bons calculs mais y a moyen d'optimiser oui^^
    double xp = point2.getX();
    double yp = point2.getY();
    double dx = xp - xi;
    double dy = yp - yi;

    double t = (dy * (xi - s.getX()) - dx * (yi - s.getY()))/(ux*dy - uy*dx);
    //std::cout << "dx = " << dx << ", dy = " << dy << std::endl;
    return Object(s.getX() + t * ux, s.getY() + t * uy);
}

Wall::Wall(double x, double y, double length, double angle, double thickness, double epsR, double sigma) :
Object(x, y), length(length), angle(angle), thickness(thickness), epsR(epsR), sigma(sigma)
{
    x2 = x + length * cos(angle * PI/180);
    y2 = y + length * sin(angle * PI/180);
    
    nx = y2 - y;
    ny = x - x2;
    double norm = sqrt(nx * nx + ny * ny);
    nx /= norm;
    ny /= norm;
    a = (y2 - y) / (x2 - x);
    b = y - a * x;
    ux = x2 - x;
    uy = y2 - y;
    norm = sqrt(ux * ux + uy * uy);
    ux /= norm;
    uy /= norm;
}