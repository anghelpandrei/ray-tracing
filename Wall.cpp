//
// Created by Andrei on 23/04/2022. Test
//

#include "Wall.h"


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

Wall::Wall(double posX, double posY, double length, double angle, double thickness, double epsR, double sigma) : posX(posX), posY(posY), length(length), angle(angle),
                                                                                       thickness(thickness), epsR(epsR),
                                                                                       sigma(sigma) {}

